#ifndef RELAYCTL_H
#define RELAYCTL_H


class RelayCtl
{
  public:
    RelayCtl(const QString device);
    ~RelayCtl();
    int status(int relayno);
  private:
    QString m_device;
    int m_relaycount;

};


#endif // RELAYCTL_H
