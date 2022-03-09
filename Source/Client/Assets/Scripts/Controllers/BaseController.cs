using System.Collections;
using System.Collections.Generic;
using System.Configuration;
using UnityEngine;
using Define;
using Info;

public class StatInfo
{
	public int HP;
	public float Speed;
}

public class BaseController : MonoBehaviour
{
	public int Id { get; set; }

	StatInfo _stat = new StatInfo();
	public virtual StatInfo Stat
	{
		get { return _stat; }
		set
		{
			if (_stat.Equals(value))
				return;

			//_stat.MergeFrom(value);
		}
	}

	public float Speed
	{
		get { return Stat.Speed; }
		set { Stat.Speed = value; }
	}

	public virtual int HP
	{
		get { return Stat.HP; }
		set
		{
			Stat.HP = value;
		}
	}

	protected bool _updated = false;

	PositionInfoT _positionInfo = new PositionInfoT();
	public PositionInfoT PosInfo
	{
		get { return _positionInfo; }
		set
		{
			if (_positionInfo.Equals(value))
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

	public Dir GetDirFromVec(Vector3Int dir)
	{
		if (dir.x > 0)
			return Dir.RIGHT;
		else if (dir.x < 0)
			return Dir.LEFT;
		else if (dir.y > 0)
			return Dir.UP;
		else
			return Dir.DOWN;
	}

	public Vector3Int GetFrontCellPos()
	{
		Vector3Int cellPos = CellPos;

		switch (Dir)
		{
			case Dir.UP:
				cellPos += Vector3Int.up;
				break;
			case Dir.DOWN:
				cellPos += Vector3Int.down;
				break;
			case Dir.LEFT:
				cellPos += Vector3Int.left;
				break;
			case Dir.RIGHT:
				cellPos += Vector3Int.right;
				break;
		}

		return cellPos;
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
		_stat.Speed = 10.0f;

		UpdateAnimation();
	}

	protected virtual void UpdateController()
	{
		switch (State)
		{
			case CreatureState.IDLE:
				UpdateIdle();
				break;
			case CreatureState.MOVE:
				UpdateMoving();
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
		if (dist < Speed * Time.deltaTime)
		{
			transform.position = destPos;
			MoveToNextPos();
		}
		else
		{
			transform.position += moveDir.normalized * Speed * Time.deltaTime;
			State = CreatureState.MOVE;
		}
	}

	protected virtual void MoveToNextPos()
	{

	}

	protected virtual void UpdateSkill()
	{

	}

	protected virtual void UpdateDead()
	{

	}
}