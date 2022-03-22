#include "ez.hpp"
#include <QDataStream>
#include <QDebug>
#include <QFile>

ez::ez(QObject *parent) : QObject{parent} {}

bool ez::compress_file(QString const &file_name) {
  QFile ofile{file_name};
  QFile ezfile{file_name + ".ez"};

  /*
  char tmp[size];
  QDataStream o_stream{&ofile};
  QDataStream ez_stream{&ezfile};
  o_stream.setVersion(o_stream.Qt_6_2);
  ez_stream.setVersion(o_stream.Qt_6_2);

  QByteArray buffer;
  QByteArray compress;
*/

  if (!ofile.open(QIODevice::ReadOnly)) {
    qCritical() << "read " << ofile.errorString();
    return false;
  }

  if (!ezfile.open(QIODevice::WriteOnly)) {
    qCritical() << "write " << ofile.errorString();
    return false;
  }

  while (!ofile.atEnd()) {

    QByteArray buffer = ofile.read(size);
    QByteArray compress = qCompress(buffer, 9);

    /*
  o_stream.readRawData(tmp, size);
  buffer.append(tmp, size);
  compress = qCompress(buffer, 9);
*/
    ezfile.write(sign);
    ezfile.write(compress);
    ezfile.flush();
  }

  ofile.close();
  ezfile.close();

  qInfo("finished compressing\n");
  return true;
}

bool ez::decompress_file(QString const &file_name, QString const &d_name) {
  QFile ezfile{file_name};

  QFile dfile{d_name};
  if (!ezfile.open(QIODevice::ReadOnly)) {
    qCritical() << ezfile.errorString();
    return false;
  }

  if (!dfile.open(QIODevice::Append)) {
    qCritical() << dfile.errorString();
    return false;
  }

  while (!ezfile.atEnd()) {
    int count{0};
    QByteArray buffer = ezfile.read(size);
    if (!buffer.startsWith(sign))
      return false;

    buffer.remove(count, sign.size());
    count += size;
    QByteArray uncompress = qUncompress(buffer);
    qDebug() << uncompress;
    dfile.write(uncompress);
    dfile.flush();
  }

  ezfile.close();
  dfile.close();

  qInfo("finished uncompressing\n");

  return true;
}
