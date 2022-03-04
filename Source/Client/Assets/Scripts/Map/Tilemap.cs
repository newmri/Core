using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Tilemap
{
    Grid<int> _grid;

    public Tilemap(Vector2Int size, float cellSize, Vector3 originPos = default(Vector3))
    {
       _grid = new Grid<int>(size, cellSize, originPos, (Grid<int> g, Vector2Int cellPos) => 0);
    }

    public class TilemapObject
    {
        public enum TilemapSprite
        {
            None,
            Ground
        }

        TilemapSprite _tilemapSprite;
        Grid<TilemapObject> _grid;
        Vector2Int _cellPos;

        TilemapObject(Grid<TilemapObject> grid, Vector2Int cellPos)
        {
            _grid = grid;
            _cellPos = cellPos;
        }

        public void SetTilemapSprite(TilemapSprite tilemapSprite)
        {
            _tilemapSprite = tilemapSprite;
           // _grid.TriggerGridObjectChanged(_cellPos);
        }
    }
}
