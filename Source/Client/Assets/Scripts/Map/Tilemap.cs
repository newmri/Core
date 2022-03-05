using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Tilemap
{
    Grid<TilemapObject> _grid;

    public Tilemap(Vector2Int size, float cellSize, Vector3 originPos = default(Vector3))
    {
        _grid = new Grid<TilemapObject>(size, cellSize, originPos, (Grid<TilemapObject> g, Vector2Int cellPos) => new TilemapObject(g, cellPos));
    }

    public void SetTilemapSprite(Vector3 worldPos, TilemapObject.TilemapSprite tilemapSprite)
    {
        TilemapObject tilemapObject = _grid.GetGridObject(worldPos);
        if (null != tilemapObject)
        {
            tilemapObject.SetTilemapSprite(tilemapSprite);
        }
    }

    public void SetTilemapVisual(TilemapVisual tilemapVisual)
    {
        tilemapVisual.SetGrid(_grid);
    }

    public class TilemapObject
    {
        public enum TilemapSprite
        {
            None,
            Ground,
            Path
        }

        TilemapSprite _tilemapSprite;
        Grid<TilemapObject> _grid;
        Vector2Int _cellPos;

        public TilemapObject(Grid<TilemapObject> grid, Vector2Int cellPos)
        {
            _grid = grid;
            _cellPos = cellPos;
        }

        public void SetTilemapSprite(TilemapSprite tilemapSprite)
        {
            _tilemapSprite = tilemapSprite;
            _grid.TriggerGrtidObjectChanged(_cellPos);
        }

        public TilemapSprite GetTilemapSprite()
        {
            return _tilemapSprite;
        }

        public override string ToString()
        {
            return _tilemapSprite.ToString();
        }
    }
}
