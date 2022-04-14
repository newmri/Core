using System.Collections;
using System.Collections.Generic;
using System.Configuration;
using UnityEngine;
using Define;
using Info;
using UnityCoreLibrary;
using TMPro;

public class BaseController : MonoBehaviour
{
	protected bool _isRunning = false;
	protected Transform _damageTextPos;
	protected Transform _bloodPos;

	[SerializeField]
	CreatureInfoT _creatureInfo = new CreatureInfoT();

	public virtual CreatureInfoT CreatureInfo
	{
		get { return _creatureInfo; }
		set
		{
			if (_creatureInfo.Equals(value))
				return;

			PosInfo = value.PosInfo;
			_creatureInfo = value;
		}
	}

	public bool IsDead()
	{
		return (State == CreatureState.DEAD);
	}

	public float MoveSpeed
	{
		get { return CreatureInfo.Speed.Value[(int)SpeedType.MOVE_SPEED]; }
		set { CreatureInfo.Speed.Value[(int)SpeedType.MOVE_SPEED] = value; }
	}

	public virtual int HP
	{
		get { return CreatureInfo.Hp; }
		set
		{
			if(0 >= value)
            {
				CreatureInfo.Hp = 0;
				OnDead();
			}

			CreatureInfo.Hp = value; 
		}
	}

	public virtual int MP
	{
		get { return CreatureInfo.Mp; }
		set { CreatureInfo.Mp = value; }
	}

	protected bool _updated = false;

	public PositionInfoT PosInfo
	{
		get { return _creatureInfo.PosInfo; }
		set
		{
			if (_creatureInfo.PosInfo.Equals(value))
				return;

			CellPos = new Vector3Int(value.Pos.X, value.Pos.Y, 0);
			State = value.State;
			Dir = value.MoveDir;
		}
	}

	public void SyncPos()
	{
		Vector3 destPos = Managers.Map.CurrentGrid.CellToWorld(CellPos) + new Vector3(0.0f, 0.5f);
		transform.position = destPos;
	}

	public Vector3Int CellPos
	{
		get
		{
			return new Vector3Int(PosInfo.Pos.X, PosInfo.Pos.Y, 0);
		}

		set
		{
			if (PosInfo.Pos.X == value.x && PosInfo.Pos.Y == value.y)
				return;

			PosInfo.Pos.X = value.x;
			PosInfo.Pos.Y = value.y;
			_updated = true;
		}
	}

	public virtual CreatureState State
	{
		get { return PosInfo.State; }
		set
		{
			if (PosInfo.State == value)
				return;

			PosInfo.State = value;

			UpdateAnimation();
			_updated = true;
		}
	}

	public Dir Dir
	{
		get { return PosInfo.MoveDir; }
		set
		{
			if (PosInfo.MoveDir == value)
				return;

			PosInfo.MoveDir = value;

			UpdateAnimation();
			_updated = true;
		}
	}

	protected virtual void UpdateAnimation()
	{
		
	}

	void Start()
	{
		Init();
	}

	void Update()
	{
		UpdateController();
	}

	protected virtual void Init()
	{
		_damageTextPos = transform.Find("DamageTextPoint");
		_bloodPos = transform.Find("BloodPoint");
		UpdateAnimation();
	}

	protected virtual void UpdateController()
	{
		switch (State)
		{
			case CreatureState.IDLE:
				UpdateIdle();
				break;
			case CreatureState.WALK:
			case CreatureState.RUN:
				UpdateMoving();
				break;
			case CreatureState.HIT:
				UpdateHit();
				break;
			case CreatureState.SKILL:
				UpdateSkill();
				break;
			case CreatureState.DEAD:
				UpdateDead();
				break;
		}
	}

	protected virtual void UpdateIdle()
	{
	}

	// 스르륵 이동하는 것을 처리
	protected virtual void UpdateMoving()
	{
		Vector3 destPos = Managers.Map.CurrentGrid.CellToWorld(CellPos) + new Vector3(0.0f, 0.5f);
		Vector3 moveDir = destPos - transform.position;

		// 도착 여부 체크
		float dist = moveDir.magnitude;
		if (dist < MoveSpeed * Time.deltaTime)
		{
			transform.position = destPos;
			MoveToNextPos();
		}
		else
		{
			transform.position += moveDir.normalized * MoveSpeed * Time.deltaTime;
			SetMoveState();
		}
	}
	
	protected void SetMoveState()
	{
		if (_isRunning)
			State = CreatureState.RUN;
		else
			State = CreatureState.WALK;
	}

	protected virtual void MoveToNextPos()
	{

	}

	protected virtual void UpdateHit()
	{

	}

	protected virtual void UpdateSkill()
	{

	}

	protected virtual void UpdateDead()
	{

	}

	public virtual void OnHit(int damage, bool isCritical)
    {
		State = CreatureState.HIT;
		HP -= damage;

		DamageText text = CoreManagers.Obj.Add("Text", "DamageText", _damageTextPos.position, 30).GetComponent<DamageText>();
		text.Damage = damage;

		CoreManagers.Obj.Add("Effect", "HitBlood", _bloodPos.position, 30);
	}

	protected virtual void OnDead()
    {
		State = CreatureState.DEAD;
	}
}