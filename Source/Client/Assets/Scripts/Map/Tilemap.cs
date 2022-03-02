using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Tilemap
{
    Grid<int> _grid;

    public Tilemap(Vector2Int size, float cellSize, Vector3 originPos = default(Vector3))
    {
       // _grid = new Grid<int>(size, cellSize, originPos, (Grid<int> g, Vector2Int cellPos) => 0);
    }
}
