// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace GamePacket
{

using global::System;
using global::System.Collections.Generic;
using global::FlatBuffers;

public enum ErrorCode : sbyte
{
  SUCCESS = 0,
  UNKNOWN = 1,
  ALREADY_LOGINED = 2,
};

public enum Packet : byte
{
  NONE = 0,
  CS_LOGIN_REQ = 1,
  SC_LOGIN_RES = 2,
  SC_PING_REQ = 3,
  CS_PING_RES = 4,
  CS_LOGOUT_NOTI = 5,
};

public class PacketUnion {
  public Packet Type { get; set; }
  public object Value { get; set; }

  public PacketUnion() {
    this.Type = Packet.NONE;
    this.Value = null;
  }

  public T As<T>() where T : class { return this.Value as T; }
  public GamePacket.CS_LOGIN_REQT AsCS_LOGIN_REQ() { return this.As<GamePacket.CS_LOGIN_REQT>(); }
  public GamePacket.SC_LOGIN_REST AsSC_LOGIN_RES() { return this.As<GamePacket.SC_LOGIN_REST>(); }
  public GamePacket.SC_PING_REQT AsSC_PING_REQ() { return this.As<GamePacket.SC_PING_REQT>(); }
  public GamePacket.CS_PING_REST AsCS_PING_RES() { return this.As<GamePacket.CS_PING_REST>(); }
  public GamePacket.CS_LOGOUT_NOTIT AsCS_LOGOUT_NOTI() { return this.As<GamePacket.CS_LOGOUT_NOTIT>(); }

  public static int Pack(FlatBuffers.FlatBufferBuilder builder, PacketUnion _o) {
    switch (_o.Type) {
      default: return 0;
      case Packet.CS_LOGIN_REQ: return GamePacket.CS_LOGIN_REQ.Pack(builder, _o.AsCS_LOGIN_REQ()).Value;
      case Packet.SC_LOGIN_RES: return GamePacket.SC_LOGIN_RES.Pack(builder, _o.AsSC_LOGIN_RES()).Value;
      case Packet.SC_PING_REQ: return GamePacket.SC_PING_REQ.Pack(builder, _o.AsSC_PING_REQ()).Value;
      case Packet.CS_PING_RES: return GamePacket.CS_PING_RES.Pack(builder, _o.AsCS_PING_RES()).Value;
      case Packet.CS_LOGOUT_NOTI: return GamePacket.CS_LOGOUT_NOTI.Pack(builder, _o.AsCS_LOGOUT_NOTI()).Value;
    }
  }
}

public struct CS_LOGIN_REQ : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_2_0_0(); }
  public static CS_LOGIN_REQ GetRootAsCS_LOGIN_REQ(ByteBuffer _bb) { return GetRootAsCS_LOGIN_REQ(_bb, new CS_LOGIN_REQ()); }
  public static CS_LOGIN_REQ GetRootAsCS_LOGIN_REQ(ByteBuffer _bb, CS_LOGIN_REQ obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public CS_LOGIN_REQ __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long Uid { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetLong(o + __p.bb_pos) : (long)0; } }
  public long CharacterUid { get { int o = __p.__offset(6); return o != 0 ? __p.bb.GetLong(o + __p.bb_pos) : (long)0; } }
  public int Token { get { int o = __p.__offset(8); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }

  public static Offset<GamePacket.CS_LOGIN_REQ> CreateCS_LOGIN_REQ(FlatBufferBuilder builder,
      long uid = 0,
      long character_uid = 0,
      int token = 0) {
    builder.StartTable(3);
    CS_LOGIN_REQ.AddCharacterUid(builder, character_uid);
    CS_LOGIN_REQ.AddUid(builder, uid);
    CS_LOGIN_REQ.AddToken(builder, token);
    return CS_LOGIN_REQ.EndCS_LOGIN_REQ(builder);
  }

  public static void StartCS_LOGIN_REQ(FlatBufferBuilder builder) { builder.StartTable(3); }
  public static void AddUid(FlatBufferBuilder builder, long uid) { builder.AddLong(0, uid, 0); }
  public static void AddCharacterUid(FlatBufferBuilder builder, long characterUid) { builder.AddLong(1, characterUid, 0); }
  public static void AddToken(FlatBufferBuilder builder, int token) { builder.AddInt(2, token, 0); }
  public static Offset<GamePacket.CS_LOGIN_REQ> EndCS_LOGIN_REQ(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<GamePacket.CS_LOGIN_REQ>(o);
  }
  public CS_LOGIN_REQT UnPack() {
    var _o = new CS_LOGIN_REQT();
    this.UnPackTo(_o);
    return _o;
  }
  public void UnPackTo(CS_LOGIN_REQT _o) {
    _o.Uid = this.Uid;
    _o.CharacterUid = this.CharacterUid;
    _o.Token = this.Token;
  }
  public static Offset<GamePacket.CS_LOGIN_REQ> Pack(FlatBufferBuilder builder, CS_LOGIN_REQT _o) {
    if (_o == null) return default(Offset<GamePacket.CS_LOGIN_REQ>);
    return CreateCS_LOGIN_REQ(
      builder,
      _o.Uid,
      _o.CharacterUid,
      _o.Token);
  }
};

public class CS_LOGIN_REQT
{
  public long Uid { get; set; }
  public long CharacterUid { get; set; }
  public int Token { get; set; }

  public CS_LOGIN_REQT() {
    this.Uid = 0;
    this.CharacterUid = 0;
    this.Token = 0;
  }
}

public struct CHARACTER_INFO : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_2_0_0(); }
  public static CHARACTER_INFO GetRootAsCHARACTER_INFO(ByteBuffer _bb) { return GetRootAsCHARACTER_INFO(_bb, new CHARACTER_INFO()); }
  public static CHARACTER_INFO GetRootAsCHARACTER_INFO(ByteBuffer _bb, CHARACTER_INFO obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public CHARACTER_INFO __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long Uid { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetLong(o + __p.bb_pos) : (long)0; } }
  public string Name { get { int o = __p.__offset(6); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetNameBytes() { return __p.__vector_as_span<byte>(6, 1); }
#else
  public ArraySegment<byte>? GetNameBytes() { return __p.__vector_as_arraysegment(6); }
#endif
  public byte[] GetNameArray() { return __p.__vector_as_array<byte>(6); }
  public byte Level { get { int o = __p.__offset(8); return o != 0 ? __p.bb.Get(o + __p.bb_pos) : (byte)0; } }
  public Define.Job Job { get { int o = __p.__offset(10); return o != 0 ? (Define.Job)__p.bb.Get(o + __p.bb_pos) : Define.Job.WARRIOR; } }
  public byte Gear(int j) { int o = __p.__offset(12); return o != 0 ? __p.bb.Get(__p.__vector(o) + j * 1) : (byte)0; }
  public int GearLength { get { int o = __p.__offset(12); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<byte> GetGearBytes() { return __p.__vector_as_span<byte>(12, 1); }
#else
  public ArraySegment<byte>? GetGearBytes() { return __p.__vector_as_arraysegment(12); }
#endif
  public byte[] GetGearArray() { return __p.__vector_as_array<byte>(12); }

  public static Offset<GamePacket.CHARACTER_INFO> CreateCHARACTER_INFO(FlatBufferBuilder builder,
      long uid = 0,
      StringOffset nameOffset = default(StringOffset),
      byte level = 0,
      Define.Job job = Define.Job.WARRIOR,
      VectorOffset gearOffset = default(VectorOffset)) {
    builder.StartTable(5);
    CHARACTER_INFO.AddUid(builder, uid);
    CHARACTER_INFO.AddGear(builder, gearOffset);
    CHARACTER_INFO.AddName(builder, nameOffset);
    CHARACTER_INFO.AddJob(builder, job);
    CHARACTER_INFO.AddLevel(builder, level);
    return CHARACTER_INFO.EndCHARACTER_INFO(builder);
  }

  public static void StartCHARACTER_INFO(FlatBufferBuilder builder) { builder.StartTable(5); }
  public static void AddUid(FlatBufferBuilder builder, long uid) { builder.AddLong(0, uid, 0); }
  public static void AddName(FlatBufferBuilder builder, StringOffset nameOffset) { builder.AddOffset(1, nameOffset.Value, 0); }
  public static void AddLevel(FlatBufferBuilder builder, byte level) { builder.AddByte(2, level, 0); }
  public static void AddJob(FlatBufferBuilder builder, Define.Job job) { builder.AddByte(3, (byte)job, 0); }
  public static void AddGear(FlatBufferBuilder builder, VectorOffset gearOffset) { builder.AddOffset(4, gearOffset.Value, 0); }
  public static VectorOffset CreateGearVector(FlatBufferBuilder builder, byte[] data) { builder.StartVector(1, data.Length, 1); for (int i = data.Length - 1; i >= 0; i--) builder.AddByte(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateGearVectorBlock(FlatBufferBuilder builder, byte[] data) { builder.StartVector(1, data.Length, 1); builder.Add(data); return builder.EndVector(); }
  public static void StartGearVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(1, numElems, 1); }
  public static Offset<GamePacket.CHARACTER_INFO> EndCHARACTER_INFO(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<GamePacket.CHARACTER_INFO>(o);
  }
  public CHARACTER_INFOT UnPack() {
    var _o = new CHARACTER_INFOT();
    this.UnPackTo(_o);
    return _o;
  }
  public void UnPackTo(CHARACTER_INFOT _o) {
    _o.Uid = this.Uid;
    _o.Name = this.Name;
    _o.Level = this.Level;
    _o.Job = this.Job;
    _o.Gear = new List<byte>();
    for (var _j = 0; _j < this.GearLength; ++_j) {_o.Gear.Add(this.Gear(_j));}
  }
  public static Offset<GamePacket.CHARACTER_INFO> Pack(FlatBufferBuilder builder, CHARACTER_INFOT _o) {
    if (_o == null) return default(Offset<GamePacket.CHARACTER_INFO>);
    var _name = _o.Name == null ? default(StringOffset) : builder.CreateString(_o.Name);
    var _gear = default(VectorOffset);
    if (_o.Gear != null) {
      var __gear = _o.Gear.ToArray();
      _gear = CreateGearVector(builder, __gear);
    }
    return CreateCHARACTER_INFO(
      builder,
      _o.Uid,
      _name,
      _o.Level,
      _o.Job,
      _gear);
  }
};

public class CHARACTER_INFOT
{
  public long Uid { get; set; }
  public string Name { get; set; }
  public byte Level { get; set; }
  public Define.Job Job { get; set; }
  public List<byte> Gear { get; set; }

  public CHARACTER_INFOT() {
    this.Uid = 0;
    this.Name = null;
    this.Level = 0;
    this.Job = Define.Job.WARRIOR;
    this.Gear = null;
  }
}

public struct SC_LOGIN_RES : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_2_0_0(); }
  public static SC_LOGIN_RES GetRootAsSC_LOGIN_RES(ByteBuffer _bb) { return GetRootAsSC_LOGIN_RES(_bb, new SC_LOGIN_RES()); }
  public static SC_LOGIN_RES GetRootAsSC_LOGIN_RES(ByteBuffer _bb, SC_LOGIN_RES obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public SC_LOGIN_RES __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public GamePacket.ErrorCode Result { get { int o = __p.__offset(4); return o != 0 ? (GamePacket.ErrorCode)__p.bb.GetSbyte(o + __p.bb_pos) : GamePacket.ErrorCode.SUCCESS; } }
  public GamePacket.CHARACTER_INFO? CharacterInfo { get { int o = __p.__offset(6); return o != 0 ? (GamePacket.CHARACTER_INFO?)(new GamePacket.CHARACTER_INFO()).__assign(__p.__indirect(o + __p.bb_pos), __p.bb) : null; } }
  public int Money(int j) { int o = __p.__offset(8); return o != 0 ? __p.bb.GetInt(__p.__vector(o) + j * 4) : (int)0; }
  public int MoneyLength { get { int o = __p.__offset(8); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<int> GetMoneyBytes() { return __p.__vector_as_span<int>(8, 4); }
#else
  public ArraySegment<byte>? GetMoneyBytes() { return __p.__vector_as_arraysegment(8); }
#endif
  public int[] GetMoneyArray() { return __p.__vector_as_array<int>(8); }

  public static Offset<GamePacket.SC_LOGIN_RES> CreateSC_LOGIN_RES(FlatBufferBuilder builder,
      GamePacket.ErrorCode result = GamePacket.ErrorCode.SUCCESS,
      Offset<GamePacket.CHARACTER_INFO> character_infoOffset = default(Offset<GamePacket.CHARACTER_INFO>),
      VectorOffset moneyOffset = default(VectorOffset)) {
    builder.StartTable(3);
    SC_LOGIN_RES.AddMoney(builder, moneyOffset);
    SC_LOGIN_RES.AddCharacterInfo(builder, character_infoOffset);
    SC_LOGIN_RES.AddResult(builder, result);
    return SC_LOGIN_RES.EndSC_LOGIN_RES(builder);
  }

  public static void StartSC_LOGIN_RES(FlatBufferBuilder builder) { builder.StartTable(3); }
  public static void AddResult(FlatBufferBuilder builder, GamePacket.ErrorCode result) { builder.AddSbyte(0, (sbyte)result, 0); }
  public static void AddCharacterInfo(FlatBufferBuilder builder, Offset<GamePacket.CHARACTER_INFO> characterInfoOffset) { builder.AddOffset(1, characterInfoOffset.Value, 0); }
  public static void AddMoney(FlatBufferBuilder builder, VectorOffset moneyOffset) { builder.AddOffset(2, moneyOffset.Value, 0); }
  public static VectorOffset CreateMoneyVector(FlatBufferBuilder builder, int[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddInt(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateMoneyVectorBlock(FlatBufferBuilder builder, int[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static void StartMoneyVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static Offset<GamePacket.SC_LOGIN_RES> EndSC_LOGIN_RES(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<GamePacket.SC_LOGIN_RES>(o);
  }
  public SC_LOGIN_REST UnPack() {
    var _o = new SC_LOGIN_REST();
    this.UnPackTo(_o);
    return _o;
  }
  public void UnPackTo(SC_LOGIN_REST _o) {
    _o.Result = this.Result;
    _o.CharacterInfo = this.CharacterInfo.HasValue ? this.CharacterInfo.Value.UnPack() : null;
    _o.Money = new List<int>();
    for (var _j = 0; _j < this.MoneyLength; ++_j) {_o.Money.Add(this.Money(_j));}
  }
  public static Offset<GamePacket.SC_LOGIN_RES> Pack(FlatBufferBuilder builder, SC_LOGIN_REST _o) {
    if (_o == null) return default(Offset<GamePacket.SC_LOGIN_RES>);
    var _character_info = _o.CharacterInfo == null ? default(Offset<GamePacket.CHARACTER_INFO>) : GamePacket.CHARACTER_INFO.Pack(builder, _o.CharacterInfo);
    var _money = default(VectorOffset);
    if (_o.Money != null) {
      var __money = _o.Money.ToArray();
      _money = CreateMoneyVector(builder, __money);
    }
    return CreateSC_LOGIN_RES(
      builder,
      _o.Result,
      _character_info,
      _money);
  }
};

public class SC_LOGIN_REST
{
  public GamePacket.ErrorCode Result { get; set; }
  public GamePacket.CHARACTER_INFOT CharacterInfo { get; set; }
  public List<int> Money { get; set; }

  public SC_LOGIN_REST() {
    this.Result = GamePacket.ErrorCode.SUCCESS;
    this.CharacterInfo = null;
    this.Money = null;
  }
}

public struct SC_PING_REQ : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_2_0_0(); }
  public static SC_PING_REQ GetRootAsSC_PING_REQ(ByteBuffer _bb) { return GetRootAsSC_PING_REQ(_bb, new SC_PING_REQ()); }
  public static SC_PING_REQ GetRootAsSC_PING_REQ(ByteBuffer _bb, SC_PING_REQ obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public SC_PING_REQ __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }


  public static void StartSC_PING_REQ(FlatBufferBuilder builder) { builder.StartTable(0); }
  public static Offset<GamePacket.SC_PING_REQ> EndSC_PING_REQ(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<GamePacket.SC_PING_REQ>(o);
  }
  public SC_PING_REQT UnPack() {
    var _o = new SC_PING_REQT();
    this.UnPackTo(_o);
    return _o;
  }
  public void UnPackTo(SC_PING_REQT _o) {
  }
  public static Offset<GamePacket.SC_PING_REQ> Pack(FlatBufferBuilder builder, SC_PING_REQT _o) {
    if (_o == null) return default(Offset<GamePacket.SC_PING_REQ>);
    StartSC_PING_REQ(builder);
    return EndSC_PING_REQ(builder);
  }
};

public class SC_PING_REQT
{

  public SC_PING_REQT() {
  }
}

public struct CS_PING_RES : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_2_0_0(); }
  public static CS_PING_RES GetRootAsCS_PING_RES(ByteBuffer _bb) { return GetRootAsCS_PING_RES(_bb, new CS_PING_RES()); }
  public static CS_PING_RES GetRootAsCS_PING_RES(ByteBuffer _bb, CS_PING_RES obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public CS_PING_RES __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }


  public static void StartCS_PING_RES(FlatBufferBuilder builder) { builder.StartTable(0); }
  public static Offset<GamePacket.CS_PING_RES> EndCS_PING_RES(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<GamePacket.CS_PING_RES>(o);
  }
  public CS_PING_REST UnPack() {
    var _o = new CS_PING_REST();
    this.UnPackTo(_o);
    return _o;
  }
  public void UnPackTo(CS_PING_REST _o) {
  }
  public static Offset<GamePacket.CS_PING_RES> Pack(FlatBufferBuilder builder, CS_PING_REST _o) {
    if (_o == null) return default(Offset<GamePacket.CS_PING_RES>);
    StartCS_PING_RES(builder);
    return EndCS_PING_RES(builder);
  }
};

public class CS_PING_REST
{

  public CS_PING_REST() {
  }
}

public struct CS_LOGOUT_NOTI : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_2_0_0(); }
  public static CS_LOGOUT_NOTI GetRootAsCS_LOGOUT_NOTI(ByteBuffer _bb) { return GetRootAsCS_LOGOUT_NOTI(_bb, new CS_LOGOUT_NOTI()); }
  public static CS_LOGOUT_NOTI GetRootAsCS_LOGOUT_NOTI(ByteBuffer _bb, CS_LOGOUT_NOTI obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public CS_LOGOUT_NOTI __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }


  public static void StartCS_LOGOUT_NOTI(FlatBufferBuilder builder) { builder.StartTable(0); }
  public static Offset<GamePacket.CS_LOGOUT_NOTI> EndCS_LOGOUT_NOTI(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<GamePacket.CS_LOGOUT_NOTI>(o);
  }
  public CS_LOGOUT_NOTIT UnPack() {
    var _o = new CS_LOGOUT_NOTIT();
    this.UnPackTo(_o);
    return _o;
  }
  public void UnPackTo(CS_LOGOUT_NOTIT _o) {
  }
  public static Offset<GamePacket.CS_LOGOUT_NOTI> Pack(FlatBufferBuilder builder, CS_LOGOUT_NOTIT _o) {
    if (_o == null) return default(Offset<GamePacket.CS_LOGOUT_NOTI>);
    StartCS_LOGOUT_NOTI(builder);
    return EndCS_LOGOUT_NOTI(builder);
  }
};

public class CS_LOGOUT_NOTIT
{

  public CS_LOGOUT_NOTIT() {
  }
}

public struct Root : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_2_0_0(); }
  public static Root GetRootAsRoot(ByteBuffer _bb) { return GetRootAsRoot(_bb, new Root()); }
  public static Root GetRootAsRoot(ByteBuffer _bb, Root obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public Root __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public GamePacket.Packet PacketType { get { int o = __p.__offset(4); return o != 0 ? (GamePacket.Packet)__p.bb.Get(o + __p.bb_pos) : GamePacket.Packet.NONE; } }
  public TTable? Packet<TTable>() where TTable : struct, IFlatbufferObject { int o = __p.__offset(6); return o != 0 ? (TTable?)__p.__union<TTable>(o + __p.bb_pos) : null; }
  public GamePacket.CS_LOGIN_REQ PacketAsCS_LOGIN_REQ() { return Packet<GamePacket.CS_LOGIN_REQ>().Value; }
  public GamePacket.SC_LOGIN_RES PacketAsSC_LOGIN_RES() { return Packet<GamePacket.SC_LOGIN_RES>().Value; }
  public GamePacket.SC_PING_REQ PacketAsSC_PING_REQ() { return Packet<GamePacket.SC_PING_REQ>().Value; }
  public GamePacket.CS_PING_RES PacketAsCS_PING_RES() { return Packet<GamePacket.CS_PING_RES>().Value; }
  public GamePacket.CS_LOGOUT_NOTI PacketAsCS_LOGOUT_NOTI() { return Packet<GamePacket.CS_LOGOUT_NOTI>().Value; }

  public static Offset<GamePacket.Root> CreateRoot(FlatBufferBuilder builder,
      GamePacket.Packet packet_type = GamePacket.Packet.NONE,
      int packetOffset = 0) {
    builder.StartTable(2);
    Root.AddPacket(builder, packetOffset);
    Root.AddPacketType(builder, packet_type);
    return Root.EndRoot(builder);
  }

  public static void StartRoot(FlatBufferBuilder builder) { builder.StartTable(2); }
  public static void AddPacketType(FlatBufferBuilder builder, GamePacket.Packet packetType) { builder.AddByte(0, (byte)packetType, 0); }
  public static void AddPacket(FlatBufferBuilder builder, int packetOffset) { builder.AddOffset(1, packetOffset, 0); }
  public static Offset<GamePacket.Root> EndRoot(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<GamePacket.Root>(o);
  }
  public static void FinishRootBuffer(FlatBufferBuilder builder, Offset<GamePacket.Root> offset) { builder.Finish(offset.Value); }
  public static void FinishSizePrefixedRootBuffer(FlatBufferBuilder builder, Offset<GamePacket.Root> offset) { builder.FinishSizePrefixed(offset.Value); }
  public RootT UnPack() {
    var _o = new RootT();
    this.UnPackTo(_o);
    return _o;
  }
  public void UnPackTo(RootT _o) {
    _o.Packet = new GamePacket.PacketUnion();
    _o.Packet.Type = this.PacketType;
    switch (this.PacketType) {
      default: break;
      case GamePacket.Packet.CS_LOGIN_REQ:
        _o.Packet.Value = this.Packet<GamePacket.CS_LOGIN_REQ>().HasValue ? this.Packet<GamePacket.CS_LOGIN_REQ>().Value.UnPack() : null;
        break;
      case GamePacket.Packet.SC_LOGIN_RES:
        _o.Packet.Value = this.Packet<GamePacket.SC_LOGIN_RES>().HasValue ? this.Packet<GamePacket.SC_LOGIN_RES>().Value.UnPack() : null;
        break;
      case GamePacket.Packet.SC_PING_REQ:
        _o.Packet.Value = this.Packet<GamePacket.SC_PING_REQ>().HasValue ? this.Packet<GamePacket.SC_PING_REQ>().Value.UnPack() : null;
        break;
      case GamePacket.Packet.CS_PING_RES:
        _o.Packet.Value = this.Packet<GamePacket.CS_PING_RES>().HasValue ? this.Packet<GamePacket.CS_PING_RES>().Value.UnPack() : null;
        break;
      case GamePacket.Packet.CS_LOGOUT_NOTI:
        _o.Packet.Value = this.Packet<GamePacket.CS_LOGOUT_NOTI>().HasValue ? this.Packet<GamePacket.CS_LOGOUT_NOTI>().Value.UnPack() : null;
        break;
    }
  }
  public static Offset<GamePacket.Root> Pack(FlatBufferBuilder builder, RootT _o) {
    if (_o == null) return default(Offset<GamePacket.Root>);
    var _packet_type = _o.Packet == null ? GamePacket.Packet.NONE : _o.Packet.Type;
    var _packet = _o.Packet == null ? 0 : GamePacket.PacketUnion.Pack(builder, _o.Packet);
    return CreateRoot(
      builder,
      _packet_type,
      _packet);
  }
};

public class RootT
{
  public GamePacket.PacketUnion Packet { get; set; }

  public RootT() {
    this.Packet = null;
  }
  public static RootT DeserializeFromBinary(byte[] fbBuffer) {
    return Root.GetRootAsRoot(new ByteBuffer(fbBuffer)).UnPack();
  }
  public byte[] SerializeToBinary() {
    var fbb = new FlatBufferBuilder(0x10000);
    Root.FinishRootBuffer(fbb, Root.Pack(fbb, this));
    return fbb.DataBuffer.ToSizedArray();
  }
}


}