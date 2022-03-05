using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using CodeMonkey.Utils;
using CodeMonkey;

public class Testing : MonoBehaviour
{
    [SerializeField]
    TilemapVisual _tilemapVisual;

    Tilemap _tilemap;
    Tilemap.TilemapObject.TilemapSprite _tilemapSprite;

    // Start is called before the first frame update
    void Start()
    {
        _tilemap = new Tilemap(new Vector2Int(20, 10), 1f);
        _tilemap.SetTilemapVisual(_tilemapVisual);
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            Vector3 mouseWorldPos = UtilsClass.GetMouseWorldPosition();
            _tilemap.SetTilemapSprite(mouseWorldPos, _tilemapSprite);
        }

        if (Input.GetKeyDown(KeyCode.T))
        {
            _tilemapSprite = Tilemap.TilemapObject.TilemapSprite.None;
            CMDebug.TextPopupMouse(_tilemapSprite.ToString());
        }
        if (Input.GetKeyDown(KeyCode.Y))
        {
            _tilemapSprite = Tilemap.TilemapObject.TilemapSprite.Ground;
            CMDebug.TextPopupMouse(_tilemapSprite.ToString());
        }
        if (Input.GetKeyDown(KeyCode.U))
        {
            _tilemapSprite = Tilemap.TilemapObject.TilemapSprite.Path;
            CMDebug.TextPopupMouse(_tilemapSprite.ToString());
        }
    }
}
