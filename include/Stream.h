#ifndef _TSTREAM_H
#define _TSTREAM_H

namespace mlib {

class Stream {
public:
	virtual void Close () = 0;
	virtual unsigned long Read (char*, int) = 0;
	virtual unsigned long Write (char*, int) = 0;
	virtual bool IsOpened () const = 0;
   virtual ~Stream();
};

}

#endif
