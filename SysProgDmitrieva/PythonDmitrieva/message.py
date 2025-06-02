import socket
import struct
from dataclasses import dataclass
from enum import IntEnum

class MessageType(IntEnum):
    MT_INIT = 0
    MT_EXIT = 1
    MT_GETDATA = 2
    MT_DATA = 3
    MT_NODATA = 4
    MT_CONFIRM = 5


class MessageRecipient(IntEnum):
    MR_BROKER = 10
    MR_ALL = 50
    MR_USER = 100


@dataclass
class MsgHeader:
    To: MessageRecipient = 0
    From: MessageRecipient = 0
    Type: MessageType = 0
    Size: int = 0

    def Send(self, s: socket.socket):
        s.send(struct.pack(f"iiii", self.To, self.From, self.Type, self.Size))

    def Receive(self, s: socket.socket):
        try:
            (self.To, self.From, self.Type, self.Size) = struct.unpack(
                "iiii", s.recv(16)
            )
        except:
            self.Size = 0
            self.Type = MessageType.MT_NODATA


class Message:
    ClientID: int = 0

    def __init__(
        self,
        To: MessageRecipient = 0,
        From: MessageRecipient = 0,
        Type: MessageType = MessageType.MT_DATA,
        Data: str = "",
    ):
        self.Header = MsgHeader(To, From, Type, len(Data) * 2)
        self.Data: str = Data

    def Send(self, s: socket.socket):
        self.Header.Send(s)
        if self.Header.Size > 0:
            s.send(struct.pack(f"{self.Header.Size}s", self.Data.encode("utf-16-le")))

    def Receive(self, s: socket.socket):
        self.Header.Receive(s)
        if self.Header.Size > 0:
            self.Data = struct.unpack(f"{self.Header.Size}s", s.recv(self.Header.Size))[
                0
            ].decode("utf-16-le")

    @staticmethod
    def SendMessage(
        s: socket.socket,
        To: MessageRecipient,
        Type: MessageType = MessageType.MT_DATA,
        Data: str = "",
    ):
        m = Message(To, Message.ClientID, Type, Data)
        m.Send(s)
        m.Receive(s)
        if m.Header.Type == MessageType.MT_INIT:
            Message.ClientID = m.Header.To
        return m