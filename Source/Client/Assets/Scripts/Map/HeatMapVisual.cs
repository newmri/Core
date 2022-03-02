using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using CodeMonkey.Utils;

public class HeatMapVisual : MonoBehaviour
{
    public int HEAT_MAP_MAX_VALUE = 100;
    public int HEAT_MAP_MIN_VALUE = 0;

    Grid<int> _grid;
    Mesh _mesh;
    bool _meshUpdate;

    private void Awake()
    {
        _mesh = new Mesh();
        GetComponent<MeshFilter>().mesh = _mesh;
    }

    public void SetGrid(Grid<int> grid)
    {
        _grid = grid;
        UpdateHeatMapVisual();
        _grid.OnGridValueChanged += OnGridValueChanged;
    }

    private void OnGridValueChanged(object sender, Grid<int>.OnGridValueChangedEventArgs e)
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

                int gridValid = _grid.GetValue(new Vector2Int(x, y));
                float gridValueNormalized = (float)gridValid / HEAT_MAP_MAX_VALUE;
                Vector2 gridValueUV = new Vector2(gridValueNormalized, 0f);
                MeshUtils.AddToMeshArrays(vertices, uv, triangles, index, _grid.GetWorldPos(x, y) + quadSize * 0.5f, 0f, quadSize, gridValueUV, gridValueUV);
            }
        }

        _mesh.vertices = vertices;
        _mesh.uv = uv;
        _mesh.triangles = triangles;
    }
}
