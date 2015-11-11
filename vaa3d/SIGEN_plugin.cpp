#include <cstdio>
#include <cassert>
#include <fstream>
#include <vector>
#include <string>

#include "basic_surf_objs.h"
#include "v3d_message.h"

#include "SIGEN_plugin.h"
#include "image3d.h"
Q_EXPORT_PLUGIN2(SIGEN, SigenPlugin);

using namespace std;

// v3d_main/basic_c_fun/v3d_interface.h
// typedef QList<V3DPluginArgItem> V3DPluginArgList;

struct input_PARA {
  QString inimg_file;
  V3DLONG channel;
};

void reconstruction_func(
    V3DPluginCallback2 &callback,
    QWidget *parent,
    input_PARA &PARA,
    bool via_gui);

QStringList SigenPlugin::menulist() const {
  return QStringList()
         << tr("Trace")
         << tr("About");
}

QStringList SigenPlugin::funclist() const {
  return QStringList()
         << tr("trace")
         << tr("help");
}

void SigenPlugin::domenu(
    const QString &menu_name,
    V3DPluginCallback2 &callback,
    QWidget *parent) {
  if (menu_name == tr("Trace")) {
    input_PARA PARA;
    reconstruction_func(callback, parent, PARA, /* via_gui = */ true);
  } else if (menu_name == tr("About")) {
    v3d_msg(tr("This plugin is porting of SIGEN (https://sites.google.com/site/sigenproject/) for Vaa3D."));
  } else {
    assert(false);
  }
}

static std::string GetArgString(const V3DPluginArgList &input, int index, int index2) {
  if (index >= input.size())
    return "";
  const std::vector<char *> &vchar = *(vector<char *> *)input[index].p;
  if (index2 >= vchar.size())
    return "";
  return vchar[index];
}

bool SigenPlugin::dofunc(
    const QString &func_name,
    const V3DPluginArgList &input,
    V3DPluginArgList &output,
    V3DPluginCallback2 &callback,
    QWidget *parent) {
  if (func_name == tr("trace")) {
    input_PARA PARA;
    vector<char *> *pinfiles = (input.size() >= 1) ? (vector<char *> *)input[0].p : 0;
    vector<char *> *pparas = (input.size() >= 2) ? (vector<char *> *)input[1].p : 0;
    vector<char *> infiles = (pinfiles != 0) ? *pinfiles : vector<char *>();
    vector<char *> paras = (pparas != 0) ? *pparas : vector<char *>();
    if (infiles.empty()) {
      fprintf(stderr, "Need input image. \n");
      return false;
    } else {
      PARA.inimg_file = infiles[0];
    }
    int k = 0;
    PARA.channel = (paras.size() >= k + 1) ? atoi(paras[k]) : 1;
    k++;
    reconstruction_func(callback, parent, PARA, /* via_gui = */ false);
  } else if (func_name == tr("help")) {
    ////HERE IS WHERE THE DEVELOPERS SHOULD UPDATE THE USAGE OF THE PLUGIN
    printf("**** Usage of SIGEN tracing **** \n");
    printf("vaa3d -x SIGEN -f trace -i <inimg_file> -p <channel> <other parameters>\n");
    printf("inimg_file       The input image\n");
    printf("channel          Data channel for tracing. Start from 1 (default 1).\n");
    printf("outswc_file      Will be named automatically based on the input image file name, so you don't have to specify it.\n\n");
  } else {
    return false;
  }
  return true;
}

image3d cvt_to_image3d(
    const unsigned char *p,
    const int unit_byte,
    const int xdim,
    const int ydim,
    const int zdim,
    const int channel_dim,
    const int channel) {
  const int stride_x = unit_byte;
  const int stride_y = unit_byte * xdim;
  const int stride_z = unit_byte * xdim * ydim;
  const int stride_c = unit_byte * xdim * ydim * zdim;
  image3d cube(xdim, ydim, zdim);
  for (int x = 0; x < xdim; ++x) {
    for (int y = 0; y < ydim; ++y) {
      for (int z = 0; z < zdim; ++z) {
        if (p[stride_x * x + stride_y * y + stride_z * z + stride_c * channel] >= 128) {
          cube(x,y,z) = true;
        } else {
          cube(x,y,z) = false;
        }
      }
    }
  }
  return cube;
}

// dump cube object to csv files.
// csv file can be visualized using tools/image_csv.py
void dump(const image3d &cube) {
  for (int z = 0; z < cube.z_; ++z) {
    char file_name[1024];
    sprintf(file_name, "/tmp/SIGEN/%04d.csv", z);
    ofstream ofs(file_name);
    for (int y = 0; y < cube.y_; ++y) {
      for (int x = 0; x < cube.x_; ++x) {
        if (x > 0)
          ofs << " ";
        ofs << cube(x,y,z);
      }
      ofs << endl;
    }
  }
}

bool sigen_config(QWidget *parent) {
  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(new QLabel(QObject::tr("foo")));
  layout->addWidget(new QLabel(QObject::tr("bar")));

  QDialog *dialog = new QDialog(parent);
  dialog->setLayout(layout);
  dialog->exec();
  return true;
}

void reconstruction_func(
    V3DPluginCallback2 &callback,
    QWidget *parent,
    input_PARA &PARA,
    bool via_gui) {
  unsigned char *data1d = NULL;
  V3DLONG N, M, P, sc, c;
  V3DLONG in_sz[4];
  if (via_gui) {
    v3dhandle curwin = callback.currentImageWindow();
    if (!curwin) {
      QMessageBox::information(0, "", "You don't have any image open in the main window.");
      return;
    }
    Image4DSimple *p4DImage = callback.getImage(curwin);
    if (!p4DImage) {
      QMessageBox::information(0, "", "The image pointer is invalid. Ensure your data is valid and try again!");
      return;
    }
    data1d = p4DImage->getRawData();
    N = p4DImage->getXDim();
    M = p4DImage->getYDim();
    P = p4DImage->getZDim();
    sc = p4DImage->getCDim();
    bool ok1;
    if (sc == 1) {
      c = 1;
      ok1 = true;
    } else {
      c = QInputDialog::getInteger(parent, "Channel", "Enter channel NO:", 1, 1, sc, 1, &ok1);
    }
    if (!ok1) {
      return;
    }
    in_sz[0] = N;
    in_sz[1] = M;
    in_sz[2] = P;
    in_sz[3] = sc;
    PARA.inimg_file = p4DImage->getFileName();
  } else {
    int datatype = 0;
    if (!simple_loadimage_wrapper(callback, PARA.inimg_file.toStdString().c_str(), data1d, in_sz, datatype)) {
      fprintf(stderr, "Error happens in reading the subject file [%s]. Exit. \n", PARA.inimg_file.toStdString().c_str());
      return;
    }
    if (PARA.channel < 1 || PARA.channel > in_sz[3]) {
      fprintf(stderr, "Invalid channel number. \n");
      return;
    }
    N = in_sz[0];
    M = in_sz[1];
    P = in_sz[2];
    sc = in_sz[3];
    c = PARA.channel;
  }
  //main neuron reconstruction code
  //// THIS IS WHERE THE DEVELOPERS SHOULD ADD THEIR OWN NEURON TRACING CODE
  sigen_config(parent);
  image3d cube = cvt_to_image3d(data1d, /* unit_byte = */ 1, N, M, P, sc, c - 1);
  // dump(cube);
  NeuronTree nt;
  QString swc_name = PARA.inimg_file + "_SIGEN.swc";
  nt.name = "SIGEN";
  writeSWC_file(swc_name.toStdString().c_str(), nt);
  if (!via_gui) {
    if (data1d) {
      delete[] data1d;
      data1d = NULL;
    }
  }
  v3d_msg(QString("Now you can drag and drop the generated swc fle [%1] into Vaa3D.").arg(swc_name.toStdString().c_str()), via_gui);
}
