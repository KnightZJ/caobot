#include "record.h"
#include <cstdio>

Record::Record(): Module(
  "record",
  "v1.0",
  __DATE__,
  __TIME__,
  "存储群聊历史记录") {  
}

bool Record::processGroupMessage(ParsedMessage& pm) {
  int msgId = Sql::instance().saveGroupMsg(pm.groupId, pm.senderId, pm.source, pm.timestamp, pm.type);
  if (pm.type & FILE_MSG)
    Sql::instance().saveFileMsg(msgId, pm.fileMsg->name, pm.fileMsg->size, pm.fileMsg->dinfo->url);
  if (pm.type & AUDIO_MSG)
    Sql::instance().saveAudioMsg(msgId, pm.audioMsg->filename, pm.audioMsg->size, pm.audioMsg->length, pm.audioMsg->url);
  if (pm.type & FORWARDED_MSG)
    ;
  if (pm.type & FLASHIMAGE_MSG) {
    if (!pm.flashImageMsg->url.has_value())
      pm.flashImageMsg->refreshInfo();
    Sql::instance().saveImageMsg(msgId, pm.flashImageMsg->id, pm.flashImageMsg->url.value_or(""));
  }
  if (pm.type & NORMAL_MSG) {
    if (!pm.text.empty())
      Sql::instance().savePlainMsg(msgId, pm.text);
    for (auto image: pm.imageMsg) {
      if (!image.url.has_value())
        image.refreshInfo();
      Sql::instance().saveImageMsg(msgId, image.id, image.url.value_or(""));
    }
  }

  return true;
}
