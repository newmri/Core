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
	protected bool _updated = false;

	protected bool _isRunning = false;
	protected Transform _damageTextPos;
	protected Transform _bloodPos;
	protected Vector3 _posCorrection = new Vector3(0.0f, 0.5f);

	[SerializeField]

	protected ObjectInfoT _objectInfo = new ObjectInfoT();

	public virtual ObjectInfoT ObjectInfo
	{
		get { return _objectInfo; }
		set
		{
			if (_objectInfo.Equals(value))
				return;

			_objectInfo = value;
			UpdateAnimation();
		}
	}

	public bool IsDead()
	{
		return (State == ObjectState.DEAD);
	}


	public PositionInfoT PosInfo
	{
		get { return _objectInfo.PosInfo; }
		set
		{
			if (_objectInfo.PosInfo.Equals(value))
				return;

			CellPos = new Vector3Int(value.Pos.X, value.Pos.Y, 0);
			State = value.State;
			Dir = value.MoveDir;
		}
	}

	public void SyncPos()
	{
		Vector3 destPos = Managers.Map.CurrentGrid.CellToWorld(CellPos) + _posCorrection;
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

	public virtual ObjectState State
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

	protected virtual bool UpdateController()
	{
		switch (State)
		{
			case ObjectState.IDLE:
				UpdateIdle();
				return true;
			case ObjectState.WALK:
			case ObjectState.RUN:
				UpdateMoving();
				return true;
		}

		return false;
	}

	protected virtual void UpdateIdle()
	{
	}


	float _moveSpeed;
	public virtual float MoveSpeed
	{
		get { return _moveSpeed; }
		set { _moveSpeed = value; }
	}

	// 스르륵 이동하는 것을 처리
	protected virtual void UpdateMoving()
	{
		Vector3 destPos = Managers.Map.CurrentGrid.CellToWorld(CellPos) + _posCorrection;
		Vector3 moveDir = destPos - transform.position;

		// 도착 여부 체크
		float dist = moveDir.magnitude;
		float runSpeed = _isRunning == true ? 1.3f : 1.0f;
		if (dist < MoveSpeed * runSpeed * Time.smoothDeltaTime)
		{
			transform.position = destPos;
			MoveToNextPos();
		}
		else
		{
			transform.position += moveDir.normalized * MoveSpeed * runSpeed * Time.smoothDeltaTime;
			SetMoveState();
		}
	}

	protected void SetMoveState()
	{
		if (_isRunning)
			State = ObjectState.RUN;
		else
			State = ObjectState.WALK;
	}

	protected virtual void MoveToNextPos()
	{

	}
}