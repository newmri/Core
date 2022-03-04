using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using CodeMonkey.Utils;

public class Testing : MonoBehaviour
{
    [SerializeField]
    HeatMapVisual heatMapVisual;

    Grid<HeatMapGridObject> _grid;
    // Start is called before the first frame update
    void Start()
    {
        //Tilemap tilemap = new Tilemap(new Vector2Int(20, 10), 10f);
        _grid = new Grid<HeatMapGridObject>(new Vector2Int(20, 10), 8f, Vector3.zero,
            (Grid<HeatMapGridObject> g, Vector2Int cellPos) => new HeatMapGridObject(g, cellPos));

        heatMapVisual.SetGrid(_grid);
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            Vector3 pos = UtilsClass.GetMouseWorldPosition();
            HeatMapGridObject obj = _grid.GetGridObject(pos);
            if (obj != null)
            {
                obj.AddValue(5);
            }
        }
    }
}
