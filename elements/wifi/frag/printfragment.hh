#ifndef CLICK_PRINTFRAGMENT_HH
#define CLICK_PRINTFRAGMENT_HH
#include <click/element.hh>
#include <clicknet/ether.h>
#include <click/etheraddress.hh>
#include <click/hashmap.hh>
CLICK_DECLS

/*
=c

Printfragment(mode, BSSID)

=s encapsulation, Wifi -> Ethernet

Turns 80211 packets into ethernet packets encapsulates packets in Ethernet header

=d

Mode is one of:
0x00 STA->STA
0x01 STA->AP
0x02 AP->STA
0x03 AP->AP

 BSSID is an ethernet address


=e


  wifi_cl :: Classifier (0/00%0c, 
                         0/04%0c,
                         0/08%0c);

  wifi_cl [0] -> Discard; //mgt 
  wifi_cl [1] -> Discard; //ctl
  wifi_cl [2] -> wifi_encap :: Printfragment() -> ...

=a

EtherEncap */

class PrintFragment : public Element { public:
  
  PrintFragment();
  ~PrintFragment();

  const char *class_name() const	{ return "PrintFragment"; }
  const char *processing() const	{ return AGNOSTIC; }
  
  int configure(Vector<String> &, ErrorHandler *);
  bool can_live_reconfigure() const	{ return true; }

  Packet *simple_action(Packet *);


  void add_handlers();


  String _label;
 private:

};

CLICK_ENDDECLS
#endif