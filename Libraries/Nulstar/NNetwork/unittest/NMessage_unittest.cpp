//
// Created by daviyang35 on 2019-01-22.
//

#include <gtest/gtest.h>
#include "NMessage.h"

TEST(NMessage, ValidateMessage) {
  QJsonParseError jsonError = {0};
  QString json("");

  QJsonDocument document = QJsonDocument::fromJson("{\n"
                                                   "    \"ProtocolVersion\": \"1.0\",\n"
                                                   "    \"MessageID\": \"45sdj8jcf8899ekffEFefee\",\n"
                                                   "    \"Timestamp\": \"1542102459\",\n"
                                                   "    \"TimeZone\": \"-4\",\n"
                                                   "    \"MessageType\": \"NegotiateConnection\",\n"
                                                   "    \"MessageData\": {\n"
                                                   "        \"CompressionRate\": \"3\",\n"
                                                   "        \"CompressionAlgorithm\": \"zlib\"\n"
                                                   "    }\n"
                                                   "}", &jsonError);
  ASSERT_TRUE(QJsonParseError::NoError == jsonError.error);
  ASSERT_FALSE(document.isNull());
  ASSERT_TRUE(NulstarNS::NMessage::fValidateMessageObject(document.object()));
}

