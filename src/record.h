#ifndef __RECORD_H__
#define __RECORD_H__

#include "module.h"
#include "sql.h"
#include "utils.h"

class Record: public Module {
public:
  Record();
  bool processGroupMessage(ParsedMessage& pm);
};


#endif