using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Define;
using Info;
using UnityCoreLibrary;

public class MyPlayerController : PlayerController
{
	bool _moveKeyPressed = false;

	public int WeaponDamage { get; private set; }
	public int ArmorDefence { get; private set; }

	protected override void Init()
	{
		base.Init();
		PositionInfoT pos = new PositionInfoT();
		pos.MoveDir = Dir.RIGHT;
		pos.State = CreatureState.IDLE;

		Vector2Int startPos = Managers.Map.StartCellPos;
		pos.Pos.X = startPos.x;
		pos.Pos.Y = startPos.y;
		PosInfo = pos;

		Vector3 newPos = Managers.Map.CurrentGrid.CellToWorld(CellPos) + new Vector3(0.0f, 0.5f);
		transform.position = newPos;

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
	}

	protected override void UpdateIdle()
	{
		// 이동 상태로 갈지 확인
		if (_moveKeyPressed)
		{
			SetMoveState();
			return;
		}

		if (_coSkillCooltime == null && Input.GetKey(KeyCode.Space))
		{
			Debug.Log("Skill !");

			//C_Skill skill = new C_Skill() { Info = new SkillInfo() };
			//skill.Info.SkillId = 2;
			//Managers.Network.Send(skill);

			//_coSkillCooltime = StartCoroutine("CoInputCooltime", 0.2f);
		}
	}

	protected override void UpdateMoving()
	{
		SetMoveState();
		base.UpdateMoving();
	}

	Coroutine _coSkillCooltime;
	IEnumerator CoInputCooltime(float time)
	{
		yield return new WaitForSeconds(time);
		_coSkillCooltime = null;
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
			CheckUpdatedFlag();
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
		}

        CheckUpdatedFlag();
	}

	protected override void CheckUpdatedFlag()
	{
		if (_updated)
		{
			//C_Move movePacket = new C_Move();
			//movePacket.PosInfo = PosInfo;
			//Managers.Network.Send(movePacket);
			_updated = false;
			PositionInfoT pos = new PositionInfoT();
			pos.Pos.X = CellPos.x;
			pos.Pos.Y = CellPos.y;
			if (_isRunning)
				pos.State = CreatureState.RUN;
			else
				pos.State = CreatureState.WALK;

			pos.MoveDir = Dir;
			PosInfo = pos;
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
}