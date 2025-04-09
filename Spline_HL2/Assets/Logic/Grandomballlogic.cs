using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Grandomballlogic : MonoBehaviour
{
    public GameObject parentObject;
    public GameObject ballPrefab;
    public int sizeofInstantiate = 5;
    public GameObject destroyerObject;
    public Button button;
    private int ballsRemaining;

    void Start()
    {
        ballsRemaining = sizeofInstantiate;
        List<Transform> childTransforms = GetChildren(parentObject.transform);
        List<Transform> selectedTransforms = new List<Transform>();

        for (int i = 0; i < sizeofInstantiate; i++)
        {
            int randomIndex = Random.Range(0, childTransforms.Count);
            selectedTransforms.Add(childTransforms[randomIndex]);
            childTransforms.RemoveAt(randomIndex);
        }

        foreach (Transform selectedTransform in selectedTransforms)
        {
            GameObject ball = Instantiate(ballPrefab, selectedTransform.position, Quaternion.identity);
            ball.GetComponent<BallLogic>().ballDestroyedEvent.AddListener(BallDestroyed);
        }

        button.interactable = false;
    }

    void BallDestroyed()
    {
        ballsRemaining--;

        if (ballsRemaining == 0)
        {
            button.interactable = true;
        }
    }

    List<Transform> GetChildren(Transform parent)
    {
        List<Transform> childTransforms = new List<Transform>();
        int childCount = parent.childCount;
        for (int i = 0; i < childCount; i++)
        {
            Transform childTransform = parent.GetChild(i);
            childTransforms.Add(childTransform);
            childTransforms.AddRange(GetChildren(childTransform));
        }
        return childTransforms;
    }
}

