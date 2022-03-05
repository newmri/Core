using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using CodeMonkey.Utils;

public class TilemapVisual : MonoBehaviour
{
    Grid<Tilemap.TilemapObject> _grid;
    Mesh _mesh;
    bool _meshUpdate;

    private void Awake()
    {
        _mesh = new Mesh();
        GetComponent<MeshFilter>().mesh = _mesh;
    }

    public void SetGrid(Grid<Tilemap.TilemapObject> grid)
    {
        _grid = grid;
        UpdateHeatMapVisual();
        _grid.OnGridObjectChanged += OnGridObjectChanged;
    }

    private void OnGridObjectChanged(object sender, Grid<Tilemap.TilemapObject>.OnGridObjectChangedEventArgs e)
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

                Tilemap.TilemapObject gridObject = _grid.GetGridObject(new Vector2Int(x, y));
                Tilemap.TilemapObject.TilemapSprite tilemapSprite = gridObject.GetTilemapSprite();
                Vector2 gridValueUV;
                if (Tilemap.TilemapObject.TilemapSprite.None == tilemapSprite)
                {
                    gridValueUV = Vector2.zero;
                    quadSize = Vector3.zero;
                }
                else
                {
                    gridValueUV = Vector2.one;
                }

                MeshUtils.AddToMeshArrays(vertices, uv, triangles, index, _grid.GetWorldPos(x, y) + quadSize * 0.5f, 0f, quadSize, gridValueUV, gridValueUV);
            }
        }

        _mesh.vertices = vertices;
        _mesh.uv = uv;
        _mesh.triangles = triangles;
    }
}