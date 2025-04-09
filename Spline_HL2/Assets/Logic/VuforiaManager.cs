using UnityEngine;
using Vuforia;

public class VuforiaManager : MonoBehaviour
{
    private VuforiaBehaviour vuforiaBehaviour;

    void Start()
    {
        vuforiaBehaviour = FindObjectOfType<VuforiaBehaviour>();
    }

    public void StartVuforiaRecognition()
    {
        if (vuforiaBehaviour != null)
        {
            vuforiaBehaviour.enabled = true;
        }
        else
        {
            Debug.LogError("VuforiaBehaviour not found. Make sure you have added ARCamera to the scene.");
        }
    }

    public void StopVuforiaRecognition()
    {
        if (vuforiaBehaviour != null)
        {
            vuforiaBehaviour.enabled = false;
        }
        else
        {
            Debug.LogError("VuforiaBehaviour not found. Make sure you have added ARCamera to the scene.");
        }
    }
}
