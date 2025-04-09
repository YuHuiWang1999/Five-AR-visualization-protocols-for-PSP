Shader "Unlit/CutoutShader"
{
    Properties{
        _MainTex("Texture", 2D) = "white" {}
    }

        SubShader{
            Tags { "Queue" = "Transparent" } // 设置渲染队列为透明队列，确保正确的渲染顺序
            Pass {
                Stencil {
                    Ref 1                // 设置Stencil参考值为1
                    Comp always         // 每次渲染都将Stencil值设置为参考值
                    Pass replace     // 每次渲染都将覆盖当前的Stencil值
                }
                CGPROGRAM
                #pragma vertex vert
                #pragma fragment frag
                #include "UnityCG.cginc"

                struct appdata {
                    float4 vertex : POSITION;
                };

                struct v2f {
                    float4 vertex : SV_POSITION;
                };

                sampler2D _MainTex;

                v2f vert(appdata v) {
                    v2f o;
                    o.vertex = UnityObjectToClipPos(v.vertex);
                    return o;
                }

                fixed4 frag(v2f i) : SV_Target {
                    return tex2D(_MainTex, i.vertex.xy);
                }
                ENDCG
            }
    }
}