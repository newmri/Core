using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using UnityEngine.Tilemaps;
using UnityCoreLibrary;
using Define;

public struct PQNode : IComparable<PQNode>
{
	public int F;
	public int G;
	public int Y;
	public int X;

	public int CompareTo(PQNode other)
	{
		if (F == other.F)
			return 0;
		return F < other.F ? 1 : -1;
	}
}

public class MapManager
{
	public static Vector2Int MinCorrections { get; private set; } = new Vector2Int(0, 0);
	public static Vector2Int MaxCorrections { get; private set; } = new Vector2Int(-1, -1);

	public Grid CurrentGrid { get; private set; }

	public Vector2Int Min { get; set; }
	public Vector2Int Max { get; set; }
	public Vector2Int Size { get { return new Vector2Int(Max.x - Min.x + 1, Max.y - Min.y + 1); } }

	PathType[,] _path;
	int[,] _objects;
	
	Vector2Int _startIndex;
	public Vector2Int StartCellPos { get; private set; }

	Tilemap _map;
	Tilemap _mapPath;
	private string _mapName;

	[SerializeField]
	private Dictionary<TileBase, TileData> _tileDataList = new Dictionary<TileBase, TileData>();

    public bool CanMove(Vector3Int worldPos, bool checkObjects = true)
	{
		Vector2Int pos = new Vector2Int(worldPos.x, worldPos.y);
		return CanMove(pos, checkObjects);
	}

	public bool CanMove(Vector2Int cellPos, bool checkObjects = true)
	{
		if (!IsValidPos(cellPos))
		{
			return false;
		}

		Vector2Int index = CellPosToIndex(cellPos);
		return (_path[index.y, index.x] == PathType.PATH || _path[index.y, index.x] == PathType.START) 
			&& (!checkObjects || _objects[index.y, index.x] == 0);
	}

	public int Find(Vector3Int cellPos)
	{
		Vector2Int pos = new Vector2Int(cellPos.x, cellPos.y);
		return Find(pos);
	}

	public int Find(Vector2Int cellPos)
	{
		if (!IsValidPos(cellPos))
		{
			return 0;
		}

		Vector2Int index = CellPosToIndex(cellPos);
		return _objects[index.y, index.x];
	}

	private bool IsValidPos(Vector2Int cellPos)
	{
		return (cellPos.x >= Min.x && cellPos.x <= Max.x && cellPos.y >= Min.y && cellPos.y <= Max.y);
	}

	public void LoadMap(int mapId)
	{
		DestroyMap();

		_mapName = "Map_" + mapId.ToString("000");
		GameObject map = CoreManagers.Obj.Add("Map", _mapName);

		LoadPath(map, "Tilemap_Path", $"Map/Data/{_mapName}");

		MakeTileData(map);
	}

	private void LoadPath(GameObject map, string name, string path)
	{
		HidePath(map, name);
		SetAnchor(map);
		CurrentGrid = map.GetComponent<Grid>();
		LoadPath(map, path);
	}

	private void HidePath(GameObject map, string name)
	{
		GameObject path = Util.FindChild(map, name, true);
		_mapPath = Util.FindChild<Tilemap>(map, name, true);

		if (path != null)
			path.SetActive(false);
	}

	private void SetAnchor(GameObject map)
	{
		List<Tilemap> tileMapList = Util.FindAllChildrens<Tilemap>(map);
		if (null == tileMapList)
			return;

		foreach (Tilemap tileMap in tileMapList)
		{
			tileMap.tileAnchor = new Vector3(0.0f, 0.0f, 0.0f);
		}
	}

	Vector2Int WorldPosToIndex(Vector3 worldPos)
	{
		Vector2Int cellPos = new Vector2Int((int)worldPos.x, (int)worldPos.y);
		return CellPosToIndex(cellPos);

	}

	Vector2Int CellPosToIndex(Vector2Int cellPos)
	{
		return new Vector2Int(cellPos.x - Min.x, Max.y - cellPos.y);
	}

	Vector2Int IndexToCellPos(Vector2Int indexPos)
	{
		return new Vector2Int(indexPos.x + Min.x, Max.y - indexPos.y);
	}

	private void LoadPath(GameObject map, string path)
	{
		StringReader reader = PreLoad(path);

		Vector2Int currCount = new Vector2Int();
		Vector2Int maxCount = new Vector2Int(Max.x - Min.x + 1, Max.y - Min.y + 1);

		_path = new PathType[maxCount.y, maxCount.x];
		_objects = new int[maxCount.y, maxCount.x];

		for (currCount.y = 0; currCount.y < maxCount.y; ++currCount.y)
		{
			string line = reader.ReadLine();
			for (currCount.x = 0; currCount.x < maxCount.x; ++currCount.x)
			{
				string value = line[currCount.x].ToString();
				_path[currCount.y, currCount.x] = (PathType)Int32.Parse(value);

				if (_path[currCount.y, currCount.x] == PathType.START)
				{
					_startIndex = new Vector2Int(currCount.x, currCount.y);
					StartCellPos = IndexToCellPos(_startIndex);
				}
			}
		}
	}

	private StringReader PreLoad(string path)
	{
		// Path 관련 파일
		TextAsset txt = CoreManagers.Resource.Load<TextAsset>(path);
		StringReader reader = new StringReader(txt.text);

		Min = new Vector2Int(int.Parse(reader.ReadLine()), int.Parse(reader.ReadLine()));
		Max = new Vector2Int(int.Parse(reader.ReadLine()), int.Parse(reader.ReadLine()));

		return reader;
	}

	private void MakeTileData(GameObject map)
	{
		_map = Util.FindChild<Tilemap>(map, "Tilemap_Base", true);

		List<TileData> tileDataList = Util.GetOrAddComponent<TileDataOwner>(map).TileDataList;

		if (null == tileDataList)
			return;

		foreach (TileData tileData in tileDataList)
		{
			foreach (TileBase tile in tileData.tiles)
			{
				for (int y = _map.cellBounds.yMax + MaxCorrections.y; y >= _map.cellBounds.yMin + MinCorrections.y; --y)
				{
					for (int x = _map.cellBounds.xMin + MinCorrections.x; x <= _map.cellBounds.xMax + MaxCorrections.x; ++x)
					{
						Vector3Int cellPos = new Vector3Int(x, y, 0);
						_map.SetTileFlags(cellPos, TileFlags.None);

						TileBase tileToAdd = _map.GetTile(cellPos);
						if (!_tileDataList.ContainsKey(tileToAdd) && tileToAdd.name == tile.name)
						{
							_tileDataList.Add(tileToAdd, tileData);
						}
					}

				}
			}
		}
	}

	public void DestroyMap()
	{
		GameObject map = GameObject.Find(_mapName);
		if (map != null)
		{
			GameObject.Destroy(map);
			CurrentGrid = null;
			_tileDataList.Clear();
		}
	}

	public void Move(int ID)
	{
		GameObject obj = CoreManagers.Obj.Find(ID);
		if (obj == null)
			return;

		Vector2Int index = WorldPosToIndex(obj.transform.position);
		_objects[index.y, index.x] = ID;
	}

	public void RemoveObject(int ID)
	{
		GameObject obj = CoreManagers.Obj.Find(ID);
		if (obj == null)
			return;

		Vector2Int index = WorldPosToIndex(obj.transform.position);
		_objects[index.y, index.x] = 0;
	}

	public TileData GetTileData(Vector2Int cellPos)
	{
		return GetTileData(new Vector3Int(cellPos.x, cellPos.y, 0));
	}

	public TileData GetTileData(Vector3Int cellPos)
	{
		TileData tileData = null;
		_tileDataList.TryGetValue(_map.GetTile(cellPos), out tileData);
		return tileData;
	}

	public TileBase GetTilePath(Vector2Int cellPos)
	{
		return GetTilePath(new Vector3Int(cellPos.x, cellPos.y, 0));
	}

	public TileBase GetTilePath(Vector3Int cellPos)
	{
		return _mapPath.GetTile(cellPos);
	}

	#region A* PathFinding

	public struct Pos
	{
		public Pos(int y, int x) { Y = y; X = x; }
		public int Y;
		public int X;

		public static bool operator ==(Pos lhs, Pos rhs)
		{
			return lhs.Y == rhs.Y && lhs.X == rhs.X;
		}

		public static bool operator !=(Pos lhs, Pos rhs)
		{
			return !(lhs == rhs);
		}

		public override bool Equals(object obj)
		{
			return (Pos)obj == this;
		}

		public override int GetHashCode()
		{
			long value = (Y << 32) | X;
			return value.GetHashCode();
		}

		public override string ToString()
		{
			return base.ToString();
		}
	}

	// U D L R
	int[] _deltaY = new int[] { 1, -1, 0, 0 };
	int[] _deltaX = new int[] { 0, 0, -1, 1 };
	int[] _cost = new int[] { 10, 10, 10, 10 };

	public List<Vector2Int> FindPath(Vector2Int startCellPos, Vector2Int destCellPos, bool checkObjects = true, int maxDist = 10)
	{
		List<Pos> path = new List<Pos>();

		// 점수 매기기
		// F = G + H
		// F = 최종 점수 (작을 수록 좋음, 경로에 따라 달라짐)
		// G = 시작점에서 해당 좌표까지 이동하는데 드는 비용 (작을 수록 좋음, 경로에 따라 달라짐)
		// H = 목적지에서 얼마나 가까운지 (작을 수록 좋음, 고정)

		// (y, x) 이미 방문했는지 여부 (방문 = closed 상태)
		HashSet<Pos> closeList = new HashSet<Pos>(); // CloseList

		// (y, x) 가는 길을 한 번이라도 발견했는지
		// 발견X => MaxValue
		// 발견O => F = G + H
		Dictionary<Pos, int> openList = new Dictionary<Pos, int>(); // OpenList
		Dictionary<Pos, Pos> parent = new Dictionary<Pos, Pos>();

		// 오픈리스트에 있는 정보들 중에서, 가장 좋은 후보를 빠르게 뽑아오기 위한 도구
		PriorityQueue<PQNode> pq = new PriorityQueue<PQNode>();

		// CellPos -> ArrayPos
		Pos pos = Cell2Pos(startCellPos);
		Pos dest = Cell2Pos(destCellPos);

		// 시작점 발견 (예약 진행)
		openList.Add(pos, 10 * (Math.Abs(dest.Y - pos.Y) + Math.Abs(dest.X - pos.X)));

		pq.Push(new PQNode() { F = 10 * (Math.Abs(dest.Y - pos.Y) + Math.Abs(dest.X - pos.X)), G = 0, Y = pos.Y, X = pos.X });
		parent.Add(pos, pos);

		while (pq.Count > 0)
		{
			// 제일 좋은 후보를 찾는다
			PQNode pqNode = pq.Pop();
			Pos node = new Pos(pqNode.Y, pqNode.X);
			// 동일한 좌표를 여러 경로로 찾아서, 더 빠른 경로로 인해서 이미 방문(closed)된 경우 스킵
			if (closeList.Contains(node))
				continue;

			// 방문한다
			closeList.Add(node);

			// 목적지 도착했으면 바로 종료
			if (node.Y == dest.Y && node.X == dest.X)
				break;

			// 상하좌우 등 이동할 수 있는 좌표인지 확인해서 예약(open)한다
			for (int i = 0; i < _deltaY.Length; i++)
			{
				Pos next = new Pos(node.Y + _deltaY[i], node.X + _deltaX[i]);

				// 너무 멀면 스킵
				if (Math.Abs(pos.Y - next.Y) + Math.Abs(pos.X - next.X) > maxDist)
					continue;

				// 유효 범위를 벗어났으면 스킵
				// 벽으로 막혀서 갈 수 없으면 스킵
				if (next.Y != dest.Y || next.X != dest.X)
				{
					if (CanMove(Pos2Cell(next), checkObjects) == false) // CellPos
						continue;
				}

				// 이미 방문한 곳이면 스킵
				if (closeList.Contains(next))
					continue;

				// 비용 계산
				int g = 0;// node.G + _cost[i];
				int h = 10 * ((dest.Y - next.Y) * (dest.Y - next.Y) + (dest.X - next.X) * (dest.X - next.X));
				// 다른 경로에서 더 빠른 길 이미 찾았으면 스킵

				int value = 0;
				if (openList.TryGetValue(next, out value) == false)
					value = Int32.MaxValue;

				if (value < g + h)
					continue;

                // 예약 진행
				if (openList.ContainsKey(next))
					openList[next] = g + h;
				else
					openList.Add(next, g + h);

				pq.Push(new PQNode() { F = g + h, G = g, Y = next.Y, X = next.X });

				if (parent.ContainsKey(next))
					parent[next] = node;
				else
					parent.Add(next, node);
            }
		}

		return CalcCellPathFromParent(parent, dest);
	}

	List<Vector2Int> CalcCellPathFromParent(Dictionary<Pos, Pos> parent, Pos dest)
	{
		List<Vector2Int> cells = new List<Vector2Int>();

		if (parent.ContainsKey(dest) == false)
		{
			Pos best = new Pos();
			int bestDist = Int32.MaxValue;

			foreach (Pos pos in parent.Keys)
			{
				int dist = Math.Abs(dest.Y - pos.Y) + Math.Abs(dest.X - pos.X);
				if (dist < bestDist)
				{
					best = pos;
					bestDist = dist;
				}
			}
			dest = best;
		}

		{
			Pos pos = dest;
			while (parent[pos] != pos)
			{
				cells.Add(Pos2Cell(pos));
				pos = parent[pos];
			}
			cells.Add(Pos2Cell(pos));
			cells.Reverse();
		}

		return cells;
	}

	Pos Cell2Pos(Vector2Int cell)
	{
		// CellPos -> ArrayPos
		return new Pos(Max.y - cell.y, cell.x - Min.x);
	}

	Vector2Int Pos2Cell(Pos pos)
	{
		// ArrayPos -> CellPos
		return new Vector2Int(pos.X + Min.x, Max.y - pos.Y);
	}
	#endregion
}
