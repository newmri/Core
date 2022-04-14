using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;

public class Effect : MonoBehaviour
{
    private ParticleSystem _particle;

    // Start is called before the first frame update
    void Start()
    {
        _particle = GetComponent<ParticleSystem>();
    }

    void FixedUpdate()
    {
        if(_particle && !_particle.IsAlive())
        {
            CoreManagers.Obj.RemoveClientObject(gameObject.GetID());
        }
    }
}
