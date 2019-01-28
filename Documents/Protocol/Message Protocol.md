# Protocol Message

[TOC]

## Common Structure

```json
{
  "MessageVersion": "1.0",
  "MessageID": "45sdj8jcf8899ekffEFefee",
  "Timestamp": 1548642550,
  "TimeZone": "+8",
  "MessageType": "",
  "MessageData": {
  } 
}
```

|Field|Means|
|---|---|
|MessageVersion|Two numbers, major and minor and follows semantic rules|
|MessageID|This is a string that identifies a request.|
|Timestamp|Number of seconds.|
|TimeZone|The time zone where the request was originated.|
|MessageType|The message type, these are specified on `Support Message Types` |
|MessageData|A JSON object that holds the payload of the message.|

> `MessageID` length should not surpass 256 characters.
> `Timestamp` since epoch January 1, 1970 at 00:00:00 GMT.
> `TimeZone` represented as a number between -12 and 12.
> `MessageData` must be a JSON object.

## Support Message Types

|Message Type|
|:---:|
|NegotiateConnection|
|NegotiateConnectionResponse|
|Request|
|Unsubscribe|
|Response|
|Ack|
|RegisterCompoundMethod|
|UnregisterCompoundMethod|

> The specific definition of the message looks at the following table.
> 

### NegotiateConnection 

This should be the first object that should be sent when establishing a connection with a microservice.
Only if the negotiation is successful. The service may process further requests.
Otherwise a NegotiateConnectionResponse object should be received with Status set to 0 (Failure) and disconnect immediately.

```json
{
  "MessageType": "NegotiateConnection",
  "MessageData": {
    "CompressionAlgorithm": "zlib",
    "CompressionRate": 4
  }
}
```

|Data Field| Means|
|---|---|
|CompressionAlgorithm|A String that represents the algorithm that will be used to receive and send messages.|
|CompressionRate|The compression level in which the messages should be sent and received for this connection.|

> `CompressionAlgorithm` currently just support zlib algorithm.
> `CompressionRate` is an integer between 0 and 9. 0 means no compression while 9 maximum compression.



### NegotiateConnectionResponse

A message of this type should be sent by a service only in response to a previous incoming NegotiateConnection message.

```json
{
  "MessageType": "NegotiateConnectionResponse",
  "MessageData": {
    "NegotiationStatus": 0,
    "NegotiationComment": "Incompatible protocol version"
  }
}
```

|Data Field| Means|
|---|---|
|NegotiationStatus|An unsigned small integer value, 0 if negotiation was a failure and 1 if it was successful|
|NegotiationComment|A string value, useful to describe what exactly went wrong when the connection was rejected.|

### Request

A caller service must send a Request object to execute a method 
offered by some service inside the Nulstar microservice’s network.
If two or more methods are enclosed in a single request object,
then the methods should be executed in sequential order 
and a Response should be sent consolidating 3 all responses.
if one of the requests fails then the whole operation is considered a failure.

```json
{
  "MessageType": "Request",
  "MessageData": {
    "RequestType": 1,
    "SubscriptionEventCounter": 3,
    "SubscriptionPeriod": 4,
    "SubscriptionRange": "[2,6)",
    "ResponseMaxSize": 2,
    "RequestMethods": [
      {
        "GetBalance": {
          "Address": "N234rFr4Rtgg5ref4x45tgg5f43335emcnd"
        }
      },
      {
        "GetHeight": {}
      }
    ]
  }
}
```

|Data Field| Means|
|---|---|
|RequestType|This is an integer that can have only 4 values. Default is 1.|
|SubscriptionEventCounter|This is an unsigned integer that specifies how many events do the target methods need to process before sending back another Response request, the first Response is always sent as soon as possible.|
|SubscriptionPeriod|This is an unsigned integer that specifies how many seconds do the target methods need to wait before sending back another Response request, the first Response is always sent as soon as possible.|
|SubscriptionRange|is a string represents the set of numbers that will trigger a Response.|
|ResponseMaxSize|An unsigned integer which specifies the maximum number of objects that the method should return, a value of 0 (the default) means no limit.|
|RequestMethods|An array that holds all methods being requested with their respective parameters.|

> `RequestType` default is 1.
> - 1: The Microserver that made the request expects only the Response.
> - 2: The Microserver that made the request expects an Ack and the Response.
> - 3: The Microserver that made the request expects only an Ack.
> - 4: The Microserver that made the request does not expect any related message.

> `SubscriptionEventCounter` For example, if the requested method is GetHeight and this parameter is set to 5 then the service will send back responses only after 5 blocks have been processed.
                             0 means the method should send a Response only once; this is the default value.

> `SubscriptionPeriod` For example, if the requested method is GetHeight and this parameter is set to 5 then the service will send back responses only after 5 seconds have passed.
                       0 means the method should send a Response only once; this is the default value.
                       
> `SubscriptionRange` If the event defined in the target microservice returns a number, this is a string that represents the set of numbers that will trigger a Response. 
> If the pair starts or ends with “(“ or “)” respectively then it means that the number is not included, if the pair pair starts or ends with “[“ or “]” respectively then it means that the number is included.
> Example: Assume we only want to be notified only when the balance is equal or greater to 1000. Then the getbalance request should be sent with “[1000, )” string as SubscriptionRange parameter.

### Unsubscribe

When a service no longer wants to receive Responses from the method it subscribed then it must send an Unsubscribe message to the target service.

```json
{
  "MessageType": "Unsubscribe",
  "MessageData": {
    "UnsubscribeMethods": [
      "GetBalance",
      "GetHeight"
    ]
  }
}
```

|Data Field| Means|
|---|---|
|UnsubscribeMethods|An array that holds all methods that the caller wants to unsubscribe|

### Response

When the target service finished processing a request, a Response should be sent with the result of the operation.

```json
{
  "MessageType": "Response",
  "MessageData": {
    "RequestID": "sdj8jcf8899ekffEFefee",
    "ResponseStatus": 1,
    "ResponseComment": "Congratulations! Processing was completed successfully!",
    "ResponseProcessingTime": 5000,
    "ResponseMaxSize": 0,
    "ResponseData": [
      {
        "Balance": 25000
      },
      {
        "Height": 1634554
      }
    ]
  }
}
```

|Data Field| Means|
|---|---|
|RequestID|This is the original request ID referred by a Request message.|
|ResponseStatus|The response status, 1 if successful, 0 otherwise.|
|ResponseComment|A string that could offer more clarification about the result of the process.|
|ResponseProcessingTime|The time that the target service took to process the request in miliseconds.|
|ResponseMaxSize|The maximum number of objects that the response contains per request.|
|ResponseData|An object array that contains the result of the method processed, one object per request.|

### Ack

This message type is sent when the RequestType is 2 or 3. Its only purpose is to notify the caller that the request was received successfully.

```json
{
  "MessageType": "Ack",
  "MessageData": {
    "RequestID": "sdj8jcf8899ekffEFefee"
  }
}
```

|Data Field| Means|
|---|---|
|RequestID|This is the original request ID referred by a Request message.|

### RegisterCompoundMethod

As explained in 3.3] a request may be composed by several methods, with this message type we register a virtual method that will execute its individual real methods in sequential order, if one of its child methods fails then the virtual method returns failure.

```json
{
  "MessageType": "RegisterCompoundMethod",
  "MessageData": {
    "CompoundMethodName": "GetMyInfo",
    "CompoundMethodDescription": "Get useful information.",
    "CompoundMethods": [
      {
        "GetBalance": {
          "Address": "GetBalanceAddress"
        }
      },
      {
        "GetHeight": {}
      }
    ]
  }
}
```

|Data Field| Means|
|---|---|
|CompoundMethodName|This is a string identifying the virtual method.|
|CompoundMethodDescription|A string describing the functionality of the method, the description will be available when querying the API for help.|
|CompoundMethods|This is an array containing the methods with their respective parameter aliases that make up the virtual method.|


### UnregisterCompoundMethod

This message type is used to unregister a compound (virtual) method.

```json
{
  "MessageType": "UnregisterCompoundMethod",
  "MessageData": {
    "UnregisterCompoundMethodName": "GetMyInfo"
  }
}
```

|Data Field| Means|
|---|---|
|UnregisterCompoundMethod|This is the string that identifies the virtual method. If it is empty then all virtual methods registered by the caller should be unregistered.|