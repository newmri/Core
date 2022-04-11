using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Define;
using Info;
using UnityCoreLibrary;
using GamePacket;
using FlatBuffers;

public class MyPlayerController : PlayerController
{
	public override CreatureInfoT CreatureInfo
	{
		get { return base.CreatureInfo; }
		set
		{
			base.CreatureInfo = value;
			_uiGameScene.UpdateHPBar(HP, base.CreatureInfo.Ability.Value[(int)Define.AbilityType.HP]);
			_uiGameScene.UpdateHPBar(MP, base.CreatureInfo.Ability.Value[(int)Define.AbilityType.MP]);
		}
	}

	public override int HP
	{
		get { return base.HP; }
		set
		{
			base.HP = value;
			_uiGameScene.UpdateHPBar(base.HP, CreatureInfo.Ability.Value[(int)Define.AbilityType.HP]);

			if (IsDead())
				Managers.UI.ShowPopupUI<UIDead>();
		}
	}

	public override int MP
	{
		get { return base.MP; }
		set
		{
			base.MP = value;
			_uiGameScene.UpdateHPBar(base.MP, CreatureInfo.Ability.Value[(int)Define.AbilityType.MP]);
		}
	}

	public MyCharacterInfoT MyCharacterInfo
	{
		get
		{
			return _characterInfo.AsMyCharacterInfo();
		}
		set
		{
			if (_characterInfo.Value != null && _characterInfo.Value.Equals(value))
				return;

			_characterInfo.Value = value;

			_gear.Job = MyCharacterInfo.Job;
			_gear.SetGear(MyCharacterInfo.Gear);
			_uiGameScene.UpdateCharacterInfo(MyCharacterInfo.Name, CreatureInfo.Level, MyCharacterInfo.Job);
		}
	}

	bool _moveKeyPressed = false;

	UIGameScene _uiGameScene;

	protected override void Init()
	{
		base.Init();
		_uiGameScene = Managers.UI.GetSceneUI<UIGameScene>();

		RefreshAdditionalStat();
	}

	protected override void UpdateController()
	{
		GetUIKeyInput();

		switch (State)
		{
			case CreatureState.IDLE:
			case CreatureState.WALK:
			case CreatureState.RUN:
				GetDirInput();
				GetRunInput();
				break;
		}

		base.UpdateController();

		UpdateSkillCoolTime();
	}

	protected override void UpdateIdle()
	{
		// 이동 상태로 갈지 확인
		if (_moveKeyPressed)
		{
			SetMoveState();
			return;
		}
	}

	protected override void UpdateMoving()
	{
		SetMoveState();
		base.UpdateMoving();
	}

	void LateUpdate()
	{
		Camera.main.transform.position = new Vector3(transform.position.x - 0.5f,
			Camera.main.transform.position.y, -10);
	}

	void GetUIKeyInput()
	{
		//if (Input.GetKeyDown(KeyCode.I))
		//{
		//	UI_GameScene gameSceneUI = Managers.UI.SceneUI as UI_GameScene;
		//	UI_Inventory invenUI = gameSceneUI.InvenUI;

		//	if (invenUI.gameObject.activeSelf)
		//	{
		//		invenUI.gameObject.SetActive(false);
		//	}
		//	else
		//	{
		//		invenUI.gameObject.SetActive(true);
		//		invenUI.RefreshUI();
		//	}
		//}
		//else if (Input.GetKeyDown(KeyCode.C))
		//{
		//	UI_GameScene gameSceneUI = Managers.UI.SceneUI as UI_GameScene;
		//	UI_Stat statUI = gameSceneUI.StatUI;

		//	if (statUI.gameObject.activeSelf)
		//	{
		//		statUI.gameObject.SetActive(false);
		//	}
		//	else
		//	{
		//		statUI.gameObject.SetActive(true);
		//		statUI.RefreshUI();
		//	}
		//}
	}

	// 키보드 입력
	void GetDirInput()
	{
		_moveKeyPressed = true;

		if (Input.GetKey(KeyCode.A))
		{
			Dir = Dir.LEFT;
		}
		else if (Input.GetKey(KeyCode.D))
		{
			Dir = Dir.RIGHT;
		}
		else
		{
			_moveKeyPressed = false;
		}
	}

	void GetRunInput()
	{
		if (Input.GetKey(KeyCode.LeftShift))
		{
			_isRunning = true;
		}
		else
		{
			_isRunning = false;
		}
	}

	protected override void MoveToNextPos()
	{
		if (_moveKeyPressed == false)
		{
			State = CreatureState.IDLE;
			SendSetState();
			return;
		}

		Vector3Int destPos = CellPos;

		switch (Dir)
		{
			case Dir.LEFT:
				destPos += Vector3Int.left;
				break;
			case Dir.RIGHT:
				destPos += Vector3Int.right;
				break;
		}

		if (Managers.Map.CanMove(destPos))
        {
			CellPos = destPos;
			CheckUpdatedFlag();
		}
	}

	void SendSetState()
    {
		FlatBufferBuilder builder = new FlatBufferBuilder(1);
		var message = CS_SET_STATE_REQ.CreateCS_SET_STATE_REQ(builder, State);
		Managers.GameNetwork.Send(builder, Packet.CS_SET_STATE_REQ, message.Value);
		_updated = false;
	}

	void SendUseSkill(int skillID)
    {
		FlatBufferBuilder builder = new FlatBufferBuilder(1);
		var message = CS_USE_SKILL_REQ.CreateCS_USE_SKILL_REQ(builder, skillID);
		Managers.GameNetwork.Send(builder, Packet.CS_USE_SKILL_REQ, message.Value);
	}

	protected override void CheckUpdatedFlag()
	{
		if (_updated)
		{
			FlatBufferBuilder builder = new FlatBufferBuilder(1);
			CS_MOVE_REQT moveReq = new CS_MOVE_REQT();
			moveReq.PosInfo = PosInfo;
			var message = CS_MOVE_REQ.Pack(builder, moveReq);
			Managers.GameNetwork.Send(builder, Packet.CS_MOVE_REQ, message.Value);
			_updated = false;
		}
	}

	public void RefreshAdditionalStat()
	{
		//WeaponDamage = 0;
		//ArmorDefence = 0;

		//foreach (Item item in Managers.Inventory.Items.Values)
		//{
		//	if (item.Equipped == false)
		//		continue;

		//	switch (item.ItemType)
		//	{
		//		case ItemType.Weapon:
		//			WeaponDamage += ((Weapon)item).Damage;
		//			break;
		//		case ItemType.Armor:
		//			ArmorDefence += ((Armor)item).Defence;
		//			break;
		//	}
		//}
	}

	public override bool UseSkill(int skillID)
	{
		if (State == CreatureState.SKILL || IsDead())
			return false;

		Skill skill = null;
		if(_skillList.TryGetValue(skillID, out skill))
        {
			if (!skill.CanUseSkill(CreatureInfo.Hp, CreatureInfo.Mp))
				return false;

			if (!base.UseSkill(skillID))
				return false;

			SendUseSkill(skillID);
			skill.Used = true;
			return true;
		}

		return false;
	}

	Dictionary<int, Skill> _skillList = new Dictionary<int, Skill>();
	public void AddSkill(int skillID)
    {
		_skillList.Add(skillID, new Skill(skillID));
	}

	void UpdateSkillCoolTime()
    {
		foreach(Skill skill in _skillList.Values)
        {
			if (skill.Used)
			{
				skill.ElapsedCoolTime += Time.deltaTime;
				if (skill.ElapsedCoolTime >= skill.CoolTime)
					skill.Init();

				_uiGameScene.UpdateSkillCoolTime(skill.SkillID, skill.ElapsedCoolTime / skill.CoolTime);
			}
		}
    }
}