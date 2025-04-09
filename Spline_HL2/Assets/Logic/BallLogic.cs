using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class BallLogic : MonoBehaviour
{
    // Start is called before the first frame update
    public UnityEvent ballDestroyedEvent;
    public GameObject boom;
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    //private void OnTriggerEnter(Collider other)
    //{
    //    Debug.Log("ballisdestroy");
    //    if (!other.name.StartsWith("probe")) return;
    //    Destroy(this.gameObject);
    //}
    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("Destroyer"))
        {
            Destroy(gameObject);
            ballDestroyedEvent.Invoke();
            Debug.Log("Ball destroyed by " + collision.gameObject.name);
            GameObject boomeffect = Instantiate(boom);
            boomeffect.transform.position = this.transform.position;

        }
    }
}
