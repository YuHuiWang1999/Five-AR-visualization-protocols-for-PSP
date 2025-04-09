using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HideAndShow : MonoBehaviour
{

    public void ShowActive()
    {
        gameObject.SetActive(true);
    }

    public void HideActive()
    {
        gameObject.SetActive(false);
    }

    public void ShowMesh()
    {
        GetComponent<Renderer>().enabled = true;
    }

    public void HideMesh()
    {
        GetComponent<Renderer>().enabled = false;
    }
}
