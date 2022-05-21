using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityCoreLibrary;

public class DamageText : MonoBehaviour
{
    float _mvoveSpeed;
    float _alphaSpeed = 1.0f;
    float LifeTime = 5.0f;
    public int Damage;

    TextMeshPro text;
    Color color;

    // Start is called before the first frame update
    void Start()
    {
        _mvoveSpeed = Random.Range(1.0f, 5.0f);

        text = GetComponent<TextMeshPro>();
        text.text = Damage.ToString();
        color = text.color;

        Invoke("DestroyObject", LifeTime);
    }

    // Update is called once per frame
    void Update()
    {
        transform.Translate(new Vector3(0, _mvoveSpeed * Time.deltaTime, 0));
        color.a = Mathf.Lerp(color.a, 0, _alphaSpeed * Time.deltaTime);
        text.color = color;
    }

    private void DestroyObject()
    {
        CoreManagers.Obj.RemoveClientObject(gameObject.GetID());
    }
}
