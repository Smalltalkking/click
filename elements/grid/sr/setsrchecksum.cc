/*
 * setsrchecksum.{cc,hh} -- element sets SR header checksum
 * John Bicket
 * apapted from setgridchecksum.{cc,hh} by Douglas S. J. De Couto
 * adapted from setipchecksum.{cc,hh} by Robert Morris
 *
 * Copyright (c) 1999-2000 Massachusetts Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

#include <click/config.h>
#include <click/confparse.hh>
#include "setsrchecksum.hh"
#include <click/glue.hh>
#include "srpacket.hh"
#include <clicknet/ether.h>
#include <clicknet/ip.h>
CLICK_DECLS

SetSRChecksum::SetSRChecksum()
{
  MOD_INC_USE_COUNT;
  add_input();
  add_output();
}

SetSRChecksum::~SetSRChecksum()
{
  MOD_DEC_USE_COUNT;
}

SetSRChecksum *
SetSRChecksum::clone() const
{
  return new SetSRChecksum();
}

Packet *
SetSRChecksum::simple_action(Packet *xp)
{
  WritablePacket *p = xp->uniqueify();
  click_ether *eh = (click_ether *) p->data();
  struct srpacket *pk = (struct srpacket *) (eh+1);
  unsigned plen = p->length();
  unsigned int tlen = 0;

  if (!pk)
    goto bad;

  if (pk->_type & PT_DATA) {
    tlen = pk->hlen_with_data();
  } else {
    tlen = pk->hlen_wo_data();
  }
  
  if (plen < sizeof(struct srpacket))
    goto bad;

  if (tlen > plen - sizeof(click_ether))
    goto bad;

  pk->_version = _sr_version;
  pk->set_checksum();

  return p;

 bad:
  click_chatter("%s: bad lengths plen %d, tlen %d", 
		id().cc(),
		plen,
		tlen);
  p->kill();
  return(0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SetSRChecksum)
