using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CubeController : MonoBehaviour
{
    private Transform transform;

    // Start is called before the first frame update
    void Start()
    {
        transform = gameObject.GetComponent<Transform>();
    }

    // Update is called once per frame
    void Update()
    {
        MoveControl();
    }

    void MoveControl()

    {

        if (Input.GetKey(KeyCode.W))
        {
            // transform.Translate(Vector3.forward * 0.1f, Space.Self);
            transform.position += Vector3.forward * 0.1f;
        }

        if (Input.GetKey(KeyCode.S))
        {
            transform.Translate(Vector3.back * 0.1f, Space.Self);
        }

        if (Input.GetKey(KeyCode.A))
        {
            transform.Translate(Vector3.left * 0.1f, Space.Self);
        }

        if (Input.GetKey(KeyCode.D))
        {
            transform.Translate(Vector3.right * 0.1f, Space.Self);
        }

        if (Input.GetKey(KeyCode.Q))
        {
            transform.Rotate(Vector3.up, -1.0f);
        }

        if (Input.GetKey(KeyCode.E))
        {
            transform.Rotate(Vector3.up, 1.0f);
        }
        // transform.Rotate(Vector3.up, Input.GetAxis("Mouse X"));
        // transform.Rotate(Vector3.left, Input.GetAxis("Mouse Y"));
    }
}
