using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CoroutineManager : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        _maxActive = 100;
    }

    public void Add(IEnumerator coroutine)
    {
        if (_numActive < _maxActive)
        {
            IEnumerator runner = CoroutineRunner(coroutine);
            _runningCoroutineList.Add(StartCoroutine(runner));
        }

        else
        {
            _queue.Enqueue(coroutine);
        }
    }

    public void Stop()
    {
        foreach (Coroutine coroutine in _runningCoroutineList)
        {
            StopCoroutine(coroutine);
        }

        _queue.Clear();
        _runningCoroutineList.Clear();
    }

    private IEnumerator CoroutineRunner(IEnumerator coroutine)
    {
        ++_numActive;

        while (coroutine.MoveNext())
        {
            yield return coroutine.Current;
        }

        --_numActive;

        if (_queue.Count > 0)
        {
            IEnumerator next = _queue.Dequeue();
            Add(next);
        }
    }

    [SerializeField]
    private int _maxActive;
    [SerializeField]
    private int _numActive;
    [SerializeField]
    private Queue<IEnumerator> _queue = new Queue<IEnumerator>();

    private List<Coroutine> _runningCoroutineList = new List<Coroutine>();
}
