
struct SocketErrorName
{
  const char* operator()(int i) const { return errors[i+1]; } 
  const char* errors [24] = {"UnknownSocketError", "ConnectionRefusedError",
                              "RemoteHostClosedError", "HostNotFoundError",
                              "SocketAccessError", "SocketResourceError",
                              "SocketTimeoutError", "DatagramTooLargeError",
                              "NetworkError", "AddressInUseError",
                              "SocketAddressNotAvailableError",
                              "UnsupportedSocketOperationError",
                              "ProxyAuthenticationRequiredError",
                              "SslHandshakeFailedError",
                              "UnfinishedSocketOperationError",
                              "ProxyConnectionRefusedError", 
                              "ProxyConnectionClosedError", 
                              "ProxyConnectionTimeoutError",
                              "ProxyNotFoundError", "ProxyProtocolError",
                              "OperationError", "SslInternalError",
                              "SslInvalidUserDataError",
                              "TemporaryError"};
};

struct SocketStateName
{
  const char* operator()(int i) const { return states[i]; } 
  const char* states [7] = {"UnconnectedState",
                              "HostLookupState",
                              "ConnectingStat",
                              "ConnectedState",
                              "BoundState",
                              "ClosingState",
                              "ListeningState"};
};
