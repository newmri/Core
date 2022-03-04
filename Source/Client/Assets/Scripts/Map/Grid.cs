using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using CodeMonkey.Utils;
using System;

public class Grid<T>
{
    Vector2Int _size;
    float _cellSize;
    T[,] _gridArray;
    TextMesh[,] _debugTextArray;
    Vector3 _originPos;

    public event EventHandler<OnGridObjectChangedEventArgs> OnGridObjectChanged;
    public class OnGridObjectChangedEventArgs : EventArgs
    {
        public Vector2Int cellPos;
    }

    public Grid(Vector2Int size, float cellSize, Vector3 originPos, Func<Grid<T>, Vector2Int, T> CreateGridObject)
    {
        _size = size;
        _cellSize = cellSize;
        _gridArray = new T[_size.x, _size.y];
        _debugTextArray = new TextMesh[_size.x, _size.y];
        _originPos = originPos;

        for (int x = 0; x < _gridArray.GetLength(0); ++x)
        {
            for (int y = 0; y < _gridArray.GetLength(1); ++y)
            {
                _gridArray[x, y] = CreateGridObject(this, new Vector2Int(x, y));
            }
        }

        for (int x = 0; x < _gridArray.GetLength(0); ++x)
        {
            for (int y = 0; y < _gridArray.GetLength(1); ++y)
            {
                _debugTextArray[x, y] = UtilsClass.CreateWorldText(_gridArray[x, y]?.ToString(), null,
                    GetWorldPos(x, y) + new Vector3(_cellSize, _cellSize) * 0.5f, 20, Color.white, TextAnchor.MiddleCenter);
                Debug.DrawLine(GetWorldPos(x, y), GetWorldPos(x, y + 1), Color.white, 100f);
                Debug.DrawLine(GetWorldPos(x, y), GetWorldPos(x + 1, y), Color.white, 100f);
            }
        }

        Debug.DrawLine(GetWorldPos(0, _size.y), GetWorldPos(_size.x, _size.y), Color.white, 100f);
        Debug.DrawLine(GetWorldPos(_size.x, 0), GetWorldPos(_size.x, _size.y), Color.white, 100f);

        OnGridObjectChanged += (object sender, OnGridObjectChangedEventArgs eventArgs) =>
        {
            _debugTextArray[eventArgs.cellPos.x, eventArgs.cellPos.y].text = _gridArray[eventArgs.cellPos.x, eventArgs.cellPos.y]?.ToString();
        };
    }

    public int GetWidth()
    {
        return _size.x;
    }

    public int GetHeight()
    {
        return _size.y;
    }

    public float GetCellSize()
    {
        return _cellSize;
    }

    public Vector3 GetWorldPos(int x, int y)
    {
        return new Vector3(x, y) * _cellSize + _originPos;
    }

    public Vector3 GetWorldPos(Vector2Int cellPos)
    {
        return new Vector3(cellPos.x, cellPos.y) * _cellSize + _originPos;
    }

    private Vector2Int GetCellPos(Vector3 WorldPos)
    {
        Vector2Int cellPos = new Vector2Int();
        cellPos.x = Mathf.FloorToInt((WorldPos - _originPos).x / _cellSize);
        cellPos.y = Mathf.FloorToInt((WorldPos - _originPos).y / _cellSize);
        return cellPos;
    }

    public bool IsValidCellPos(Vector2Int cellPos)
    {
        return (cellPos.x >= 0 && cellPos.y >= 0 && cellPos.x < _size.x && cellPos.y < _size.y);
    }



    public void SetGridObject(Vector2Int cellPos, T value)
    {
        if (!IsValidCellPos(cellPos))
            return;

        _gridArray[cellPos.x, cellPos.y] = value;
        if (OnGridObjectChanged != null)
            OnGridObjectChanged(this, new OnGridObjectChangedEventArgs { cellPos = cellPos });
    }

    public void TriggerGrtidObjectChanged(Vector2Int cellPos)
    {
        if (OnGridObjectChanged != null)
            OnGridObjectChanged(this, new OnGridObjectChangedEventArgs { cellPos = cellPos });
    }

    public void SetGridObject(Vector3 WorldPos, T value)
    {
        SetGridObject(GetCellPos(WorldPos), value);
    }

    public T GetGridObject(Vector2Int cellPos)
    {
        if (!IsValidCellPos(cellPos))
            return default(T);

        return _gridArray[cellPos.x, cellPos.y];
    }

    public T GetGridObject(Vector3 WorldPos)
    {
        return GetGridObject(GetCellPos(WorldPos));
    }
}
