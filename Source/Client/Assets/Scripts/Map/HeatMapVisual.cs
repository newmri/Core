using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using CodeMonkey.Utils;

public class HeatMapVisual : MonoBehaviour
{
    Grid<HeatMapGridObject> _grid;
    Mesh _mesh;
    bool _meshUpdate;

    private void Awake()
    {
        _mesh = new Mesh();
        GetComponent<MeshFilter>().mesh = _mesh;
    }

    public void SetGrid(Grid<HeatMapGridObject> grid)
    {
        _grid = grid;
        UpdateHeatMapVisual();
        _grid.OnGridObjectChanged += OnGridObjectChanged;
    }

    private void OnGridObjectChanged(object sender, Grid<HeatMapGridObject>.OnGridObjectChangedEventArgs e)
    {
        _meshUpdate = true;
    }

    private void LateUpdate()
    {
        if (_meshUpdate)
        {
            UpdateHeatMapVisual();
            _meshUpdate = false;
        }
    }

    private void UpdateHeatMapVisual()
    {
        MeshUtils.CreateEmptyMeshArrays(_grid.GetWidth() * _grid.GetHeight(), out Vector3[] vertices, out Vector2[] uv, out int[] triangles);

        for (int x = 0; x < _grid.GetWidth(); ++x)
        {
            for (int y = 0; y < _grid.GetHeight(); ++y)
            {
                int index = x * _grid.GetHeight() + y;
                Vector3 quadSize = new Vector3(1, 1) * _grid.GetCellSize();

                HeatMapGridObject gridObject = _grid.GetGridObject(new Vector2Int(x, y));
                float gridValueNormalized = gridObject.GetValueNormalized();
                Vector2 gridValueUV = new Vector2(gridValueNormalized, 0f);
                MeshUtils.AddToMeshArrays(vertices, uv, triangles, index, _grid.GetWorldPos(x, y) + quadSize * 0.5f, 0f, quadSize, gridValueUV, gridValueUV);
            }
        }

        _mesh.vertices = vertices;
        _mesh.uv = uv;
        _mesh.triangles = triangles;
    }
}

public class HeatMapGridObject
{
    int MIN = 0;
    int MAX = 100;

    Grid<HeatMapGridObject> _grid;
    Vector2Int _cellPos;
    public int value;

    public HeatMapGridObject(Grid<HeatMapGridObject> grid, Vector2Int cellPos)
    {
        _grid = grid;
        _cellPos = cellPos;
    }

    public void AddValue(int addValue)
    {
        value = Mathf.Clamp(value + addValue, MIN, MAX);
        _grid.TriggerGrtidObjectChanged(_cellPos);
    }

    public float GetValueNormalized()
    {
        return (float)value / MAX;
    }

    public override string ToString()
    {
        return value.ToString();
    }
}