// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_LOGINPROTOCOL_LOGIN_H_
#define FLATBUFFERS_GENERATED_LOGINPROTOCOL_LOGIN_H_

#include "flatbuffers/flatbuffers.h"

namespace Login {

struct CS_LOGIN_REQ;
struct CS_LOGIN_REQBuilder;

struct SC_LOGIN_RES;
struct SC_LOGIN_RESBuilder;

struct Root;
struct RootBuilder;

enum Packet : uint8_t {
  Packet_NONE = 0,
  Packet_CS_LOGIN_REQ = 1,
  Packet_SC_LOGIN_RES = 2,
  Packet_MIN = Packet_NONE,
  Packet_MAX = Packet_SC_LOGIN_RES
};

inline const Packet (&EnumValuesPacket())[3] {
  static const Packet values[] = {
    Packet_NONE,
    Packet_CS_LOGIN_REQ,
    Packet_SC_LOGIN_RES
  };
  return values;
}

inline const char * const *EnumNamesPacket() {
  static const char * const names[4] = {
    "NONE",
    "CS_LOGIN_REQ",
    "SC_LOGIN_RES",
    nullptr
  };
  return names;
}

inline const char *EnumNamePacket(Packet e) {
  if (flatbuffers::IsOutRange(e, Packet_NONE, Packet_SC_LOGIN_RES)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesPacket()[index];
}

template<typename T> struct PacketTraits {
  static const Packet enum_value = Packet_NONE;
};

template<> struct PacketTraits<Login::CS_LOGIN_REQ> {
  static const Packet enum_value = Packet_CS_LOGIN_REQ;
};

template<> struct PacketTraits<Login::SC_LOGIN_RES> {
  static const Packet enum_value = Packet_SC_LOGIN_RES;
};

bool VerifyPacket(flatbuffers::Verifier &verifier, const void *obj, Packet type);
bool VerifyPacketVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types);

struct CS_LOGIN_REQ FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef CS_LOGIN_REQBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           verifier.EndTable();
  }
};

struct CS_LOGIN_REQBuilder {
  typedef CS_LOGIN_REQ Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(CS_LOGIN_REQ::VT_NAME, name);
  }
  explicit CS_LOGIN_REQBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<CS_LOGIN_REQ> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<CS_LOGIN_REQ>(end);
    return o;
  }
};

inline flatbuffers::Offset<CS_LOGIN_REQ> CreateCS_LOGIN_REQ(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0) {
  CS_LOGIN_REQBuilder builder_(_fbb);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<CS_LOGIN_REQ> CreateCS_LOGIN_REQDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return Login::CreateCS_LOGIN_REQ(
      _fbb,
      name__);
}

struct SC_LOGIN_RES FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef SC_LOGIN_RESBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_RESULT = 4
  };
  bool result() const {
    return GetField<uint8_t>(VT_RESULT, 0) != 0;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_RESULT) &&
           verifier.EndTable();
  }
};

struct SC_LOGIN_RESBuilder {
  typedef SC_LOGIN_RES Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_result(bool result) {
    fbb_.AddElement<uint8_t>(SC_LOGIN_RES::VT_RESULT, static_cast<uint8_t>(result), 0);
  }
  explicit SC_LOGIN_RESBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<SC_LOGIN_RES> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<SC_LOGIN_RES>(end);
    return o;
  }
};

inline flatbuffers::Offset<SC_LOGIN_RES> CreateSC_LOGIN_RES(
    flatbuffers::FlatBufferBuilder &_fbb,
    bool result = false) {
  SC_LOGIN_RESBuilder builder_(_fbb);
  builder_.add_result(result);
  return builder_.Finish();
}

struct Root FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef RootBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PACKET_TYPE = 4,
    VT_PACKET = 6
  };
  Login::Packet packet_type() const {
    return static_cast<Login::Packet>(GetField<uint8_t>(VT_PACKET_TYPE, 0));
  }
  const void *packet() const {
    return GetPointer<const void *>(VT_PACKET);
  }
  template<typename T> const T *packet_as() const;
  const Login::CS_LOGIN_REQ *packet_as_CS_LOGIN_REQ() const {
    return packet_type() == Login::Packet_CS_LOGIN_REQ ? static_cast<const Login::CS_LOGIN_REQ *>(packet()) : nullptr;
  }
  const Login::SC_LOGIN_RES *packet_as_SC_LOGIN_RES() const {
    return packet_type() == Login::Packet_SC_LOGIN_RES ? static_cast<const Login::SC_LOGIN_RES *>(packet()) : nullptr;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_PACKET_TYPE) &&
           VerifyOffset(verifier, VT_PACKET) &&
           VerifyPacket(verifier, packet(), packet_type()) &&
           verifier.EndTable();
  }
};

template<> inline const Login::CS_LOGIN_REQ *Root::packet_as<Login::CS_LOGIN_REQ>() const {
  return packet_as_CS_LOGIN_REQ();
}

template<> inline const Login::SC_LOGIN_RES *Root::packet_as<Login::SC_LOGIN_RES>() const {
  return packet_as_SC_LOGIN_RES();
}

struct RootBuilder {
  typedef Root Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_packet_type(Login::Packet packet_type) {
    fbb_.AddElement<uint8_t>(Root::VT_PACKET_TYPE, static_cast<uint8_t>(packet_type), 0);
  }
  void add_packet(flatbuffers::Offset<void> packet) {
    fbb_.AddOffset(Root::VT_PACKET, packet);
  }
  explicit RootBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<Root> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Root>(end);
    return o;
  }
};

inline flatbuffers::Offset<Root> CreateRoot(
    flatbuffers::FlatBufferBuilder &_fbb,
    Login::Packet packet_type = Login::Packet_NONE,
    flatbuffers::Offset<void> packet = 0) {
  RootBuilder builder_(_fbb);
  builder_.add_packet(packet);
  builder_.add_packet_type(packet_type);
  return builder_.Finish();
}

inline bool VerifyPacket(flatbuffers::Verifier &verifier, const void *obj, Packet type) {
  switch (type) {
    case Packet_NONE: {
      return true;
    }
    case Packet_CS_LOGIN_REQ: {
      auto ptr = reinterpret_cast<const Login::CS_LOGIN_REQ *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case Packet_SC_LOGIN_RES: {
      auto ptr = reinterpret_cast<const Login::SC_LOGIN_RES *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return true;
  }
}

inline bool VerifyPacketVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyPacket(
        verifier,  values->Get(i), types->GetEnum<Packet>(i))) {
      return false;
    }
  }
  return true;
}

inline const Login::Root *GetRoot(const void *buf) {
  return flatbuffers::GetRoot<Login::Root>(buf);
}

inline const Login::Root *GetSizePrefixedRoot(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<Login::Root>(buf);
}

inline bool VerifyRootBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Login::Root>(nullptr);
}

inline bool VerifySizePrefixedRootBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<Login::Root>(nullptr);
}

inline void FinishRootBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Login::Root> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedRootBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Login::Root> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace Login

#endif  // FLATBUFFERS_GENERATED_LOGINPROTOCOL_LOGIN_H_