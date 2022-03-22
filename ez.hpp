#ifndef EZ_HPP
#define EZ_HPP

#include <QObject>

class ez : public QObject {
  Q_OBJECT
public:
  explicit ez(QObject *parent = nullptr);
  bool compress_file(QString const &);
  bool decompress_file(QString const &, QString const &);

private:
  QByteArray const static inline sign{"Ez0xeZ"};
  constexpr static inline int size{1024};
signals:
};

#endif // EZ_HPP
