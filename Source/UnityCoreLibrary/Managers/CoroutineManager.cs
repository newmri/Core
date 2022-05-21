using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace UnityCoreLibrary
{
    public class CoroutineManager : MonoBehaviour
    {
        // Start is called before the first frame update
        void Start()
        {
            _maxActive = 1024;
        }

        public Coroutine Add(IEnumerator coroutine)
        {
            Coroutine retCorutine = null;

            if (_numActive < _maxActive)
            {
                IEnumerator runner = CoroutineRunner(coroutine);
                retCorutine = StartCoroutine(runner);
                _runningCoroutineList.Add(retCorutine);
            }

            else
            {
                _queue.Enqueue(coroutine);
            }

            return retCorutine;
        }

        public void Stop()
        {
            foreach (Coroutine coroutine in _runningCoroutineList)
            {
                StopCoroutine(coroutine);
            }

            _numActive = 0;

            _queue.Clear();
            _runningCoroutineList.Clear();
        }

        public void Stop(Coroutine coroutine)
        {
            StopCoroutine(coroutine);
            _runningCoroutineList.Remove(coroutine);
            --_numActive;
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
}