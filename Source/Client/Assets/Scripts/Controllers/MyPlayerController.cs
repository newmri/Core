using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Define;
using UnityCoreLibrary;

public class MyPlayerController : PlayerController
{
	bool _moveKeyPressed = false;
	public int WeaponDamage { get; private set; }
	public int ArmorDefence { get; private set; }

	protected override void Init()
	{
		base.Init();
		PositionInfo pos = new PositionInfo();
		pos.MoveDir = Dir.RIGHT;
		pos.State = CreatureState.IDLE;
		pos.Pos = new Vector2Int((int)transform.position.x, (int)-8);
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
				GetDirInput();
				break;
			case CreatureState.MOVE:
				GetDirInput();
				break;
		}

		base.UpdateController();
	}

	protected override void UpdateIdle()
	{
		// 이동 상태로 갈지 확인
		if (_moveKeyPressed)
		{
			State = CreatureState.MOVE;
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
			PositionInfo pos = new PositionInfo();
			pos.Pos.x = CellPos.x;
			pos.Pos.y = CellPos.y;
			pos.State = CreatureState.MOVE;
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