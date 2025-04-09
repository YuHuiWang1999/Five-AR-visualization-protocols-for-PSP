using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Line : MonoBehaviour
{
    // Start is called before the first frame update
    public GameObject object01;
    public GameObject object02;
    void Start()
    {


    }

    // Update is called once per frame
    void Update()
    {
        LineRenderer lineRenderer = GetComponent<LineRenderer>();
        lineRenderer.positionCount = 2;
        lineRenderer.SetPosition(0, object01.transform.position);
        lineRenderer.SetPosition(1, object02.transform.position);
    }
}
