using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using CodeMonkey.Utils;

public class Testing : MonoBehaviour
{
    [SerializeField]
    HeatMapVisual heatMapVisual;

    Grid<int> _grid;

    // Start is called before the first frame update
    void Start()
    {
        _grid = new Grid<int>(new Vector2Int(20, 10), 8f, Vector3.zero);
        heatMapVisual.SetGrid(_grid);
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            Vector3 WorldPos = UtilsClass.GetMouseWorldPosition();
            int value = _grid.GetValue(WorldPos);
            _grid.SetValue(WorldPos, value + 5);
        }
    }
}
