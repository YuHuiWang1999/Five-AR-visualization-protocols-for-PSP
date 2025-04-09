using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ImageChange : MonoBehaviour
{
    public Image image;
    public Sprite[] sprites;
    public Material[] materials;
    public GameObject [] Points;
    public Material[] PointsMaterials;

    private int currentSpriteIndex = 0;
    // Start is called before the first frame update
    void Start()
    {
        currentSpriteIndex = 0;
        image.sprite = sprites[currentSpriteIndex];
        //���õ���0�����
        for (int i = 0; i < Points.Length; i++)
        {
            Renderer render = Points[i].GetComponent<Renderer>();
            render.material = PointsMaterials[0];
        }
        Renderer render0 = Points[0].GetComponent<Renderer>();
        render0.material = PointsMaterials[1];
    }
    public void materialreset()
    {
        for (int i = 0; i < Points.Length; i++)
        {
            Renderer render = Points[i].GetComponent<Renderer>();
            render.material = PointsMaterials[0];
        }
        Renderer render0 = Points[0].GetComponent<Renderer>();
        render0.material = PointsMaterials[1];
        image.sprite = sprites[0];
        currentSpriteIndex = 0;
    }
    // Update is called once per frame
    void Update()
    {
        
    }
    public void ChangeImage()
    {
        currentSpriteIndex++;//��1-8
        Debug.Log("����Ϊ" + currentSpriteIndex);

        image.sprite = sprites[currentSpriteIndex];

        // ����Image��Sprite
        for (int i = 0; i < Points.Length; i++)
        {
            // ����״ֵ̬������ɫ
            if (i < currentSpriteIndex)
            {
                Renderer render0 = Points[i].GetComponent<Renderer>();
                render0.material = PointsMaterials[2];
            }
            else if (i == currentSpriteIndex) 
            {

                Debug.Log("����" + i+"����Ϊ��ɫ");
                Renderer render0 = Points[i].GetComponent<Renderer>();
                render0.material = PointsMaterials[1];
            }
            else
            {
                Renderer render0 = Points[i].GetComponent<Renderer>();
                render0.material = PointsMaterials[0];
            }
        }
    }
}
