using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityCoreLibrary;

public class DamageText : MonoBehaviour
{
    public float MoveSpeed = 1.0f;
    public float AlphaSpeed = 1.0f;
    public float LifeTime = 5.0f;
    public int Damage;

    TextMeshPro text;
    Color color;

    // Start is called before the first frame update
    void Start()
    {
        text = GetComponent<TextMeshPro>();
        text.text = Damage.ToString();
        color = text.color;

        Invoke("DestroyObject", LifeTime);
    }

    // Update is called once per frame
    void Update()
    {
        transform.Translate(new Vector3(0, MoveSpeed * Time.deltaTime, 0));
        color.a = Mathf.Lerp(color.a, 0, AlphaSpeed * Time.deltaTime);
        text.color = color;
    }

    private void DestroyObject()
    {
        CoreManagers.Obj.RemoveClientObject(gameObject.GetID());
    }
}
