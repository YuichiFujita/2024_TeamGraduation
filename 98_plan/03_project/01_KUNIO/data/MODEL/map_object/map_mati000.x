xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 103;
 -249.05308;23.49697;-1468.56135;,
 -221.03553;-117.92746;-1468.56135;,
 -284.43296;-200.29152;-1468.56135;,
 -293.71233;-200.29152;-1468.56135;,
 208.77044;-117.92746;-1468.56135;,
 272.63455;-200.29152;-1468.56135;,
 234.26216;23.49697;-1468.56135;,
 278.92144;-200.29152;-1468.56135;,
 374.14593;23.49697;-1468.56135;,
 353.67713;-200.29152;-1468.56135;,
 400.99229;73.09039;-1468.56135;,
 400.99229;-200.29152;-1468.56135;,
 712.08941;73.09039;-1468.56135;,
 712.08941;-200.29152;-1468.56135;,
 712.08941;73.09039;-1468.56135;,
 712.08941;73.09039;1468.56135;,
 712.08941;-200.29152;1468.56135;,
 712.08941;-200.29152;-1468.56135;,
 712.08941;73.09039;1468.56135;,
 400.99229;73.09039;1468.56135;,
 400.99229;-200.29152;1468.56135;,
 712.08941;-200.29152;1468.56135;,
 374.14593;23.49697;1468.56135;,
 353.67713;-200.29152;1468.56135;,
 234.26216;23.49697;1468.56135;,
 278.92144;-200.29152;1468.56135;,
 208.77044;-117.92746;1468.56135;,
 272.63455;-200.29152;1468.56135;,
 -221.03553;-117.92746;1468.56135;,
 -284.43296;-200.29152;1468.56135;,
 -249.05308;23.49697;1468.56135;,
 -293.71233;-200.29152;1468.56135;,
 -249.05308;23.49697;1468.56135;,
 -221.03553;-117.92746;1468.56135;,
 -221.03553;-117.92746;-1468.56135;,
 -249.05308;23.49697;-1468.56135;,
 -221.03553;-117.92746;1468.56135;,
 208.77044;-117.92746;1468.56135;,
 208.77044;-117.92746;-1468.56135;,
 -221.03553;-117.92746;-1468.56135;,
 208.77044;-117.92746;1468.56135;,
 234.26216;23.49697;1468.56135;,
 234.26216;23.49697;-1468.56135;,
 208.77044;-117.92746;-1468.56135;,
 234.26216;23.49697;1468.56135;,
 374.14593;23.49697;1468.56135;,
 374.14593;23.49697;-1468.56135;,
 234.26216;23.49697;-1468.56135;,
 374.14593;23.49697;1468.56135;,
 400.99229;73.09039;1468.56135;,
 400.99229;73.09039;-1468.56135;,
 374.14593;23.49697;-1468.56135;,
 400.99229;73.09039;1468.56135;,
 712.08941;73.09039;-1468.56135;,
 400.99229;73.09039;-1468.56135;,
 -293.71233;-200.29152;-1468.56135;,
 -284.43296;-200.29152;-1468.56135;,
 -284.43296;-200.29152;1468.56135;,
 -293.71233;-200.29152;1468.56135;,
 272.63455;-200.29152;-1468.56135;,
 272.63455;-200.29152;1468.56135;,
 278.92144;-200.29152;-1468.56135;,
 278.92144;-200.29152;1468.56135;,
 353.67713;-200.29152;-1468.56135;,
 353.67713;-200.29152;1468.56135;,
 400.99229;-200.29152;-1468.56135;,
 400.99229;-200.29152;1468.56135;,
 712.08941;-200.29152;-1468.56135;,
 -249.05308;23.49697;-1468.56135;,
 -293.71233;-200.29152;-1468.56135;,
 -371.46031;-200.29152;-1468.56135;,
 -391.92930;23.49697;-1468.56135;,
 -418.77593;-200.29152;-1468.56135;,
 -418.77593;73.09039;-1468.56135;,
 -756.47111;-200.29152;-1468.56135;,
 -756.47111;73.09039;-1468.56135;,
 -756.47111;73.09039;-1468.56135;,
 -756.47111;-200.29152;-1468.56135;,
 -756.47111;-200.29152;1468.56135;,
 -756.47111;73.09039;1468.56135;,
 -756.47111;73.09039;1468.56135;,
 -756.47111;-200.29152;1468.56135;,
 -418.77593;-200.29152;1468.56135;,
 -418.77593;73.09039;1468.56135;,
 -371.46031;-200.29152;1468.56135;,
 -391.92930;23.49697;1468.56135;,
 -249.05308;23.49697;1468.56135;,
 -249.05308;23.49697;-1468.56135;,
 -391.92930;23.49697;-1468.56135;,
 -391.92930;23.49697;1468.56135;,
 -391.92930;23.49697;1468.56135;,
 -391.92930;23.49697;-1468.56135;,
 -418.77593;73.09039;-1468.56135;,
 -418.77593;73.09039;1468.56135;,
 -418.77593;73.09039;1468.56135;,
 -418.77593;73.09039;-1468.56135;,
 -756.47111;73.09039;-1468.56135;,
 -293.71233;-200.29152;-1468.56135;,
 -371.46031;-200.29152;1468.56135;,
 -371.46031;-200.29152;-1468.56135;,
 -418.77593;-200.29152;1468.56135;,
 -418.77593;-200.29152;-1468.56135;,
 -756.47111;-200.29152;-1468.56135;;
 
 38;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;14,15,16,17;,
 4;18,19,20,21;,
 4;19,22,23,20;,
 4;22,24,25,23;,
 4;24,26,27,25;,
 4;26,28,29,27;,
 4;28,30,31,29;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;40,41,42,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;52,15,53,54;,
 4;55,56,57,58;,
 4;56,59,60,57;,
 4;59,61,62,60;,
 4;61,63,64,62;,
 4;63,65,66,64;,
 4;65,67,16,66;,
 4;68,69,70,71;,
 4;71,70,72,73;,
 4;73,72,74,75;,
 4;76,77,78,79;,
 4;80,81,82,83;,
 4;83,82,84,85;,
 4;85,84,58,86;,
 4;30,87,88,89;,
 4;90,91,92,93;,
 4;94,95,96,79;,
 4;97,31,98,99;,
 4;99,98,100,101;,
 4;101,100,78,102;;
 
 MeshMaterialList {
  4;
  38;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  0,
  1,
  0,
  2,
  0,
  2,
  3,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  0,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\ishigaki000.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\ishigaki000.png";
   }
  }
  Material {
   0.627200;0.499200;0.163200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  12;
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.980936;0.194333;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.984140;0.177391;0.000000;,
  -0.000000;1.000000;0.000000;,
  -0.879416;0.476054;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  0.879414;0.476057;0.000000;;
  38;
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;5,5,5,5;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;5,5,5,5;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;;
 }
 MeshTextureCoords {
  103;
  0.333330;0.000000;,
  0.444440;0.000000;,
  0.444440;1.000000;,
  0.333330;1.000000;,
  0.555560;0.000000;,
  0.555560;1.000000;,
  0.666670;0.000000;,
  0.666670;1.000000;,
  0.777780;0.000000;,
  0.777780;1.000000;,
  0.888890;0.000000;,
  0.888890;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.111110;0.000000;,
  0.111110;1.000000;,
  0.000000;1.000000;,
  0.222220;0.000000;,
  0.222220;1.000000;,
  0.333330;0.000000;,
  0.333330;1.000000;,
  0.444440;0.000000;,
  0.444440;1.000000;,
  0.555560;0.000000;,
  0.555560;1.000000;,
  0.666670;0.000000;,
  0.666670;1.000000;,
  1.741721;0.431898;,
  1.741721;0.841794;,
  -0.741721;0.841794;,
  -0.741721;0.431898;,
  0.327442;-0.740799;,
  0.662983;-0.740799;,
  0.662983;1.742540;,
  0.327442;1.742540;,
  1.741721;0.841794;,
  1.741721;0.431898;,
  -0.741721;0.431898;,
  -0.741721;0.841794;,
  0.666670;0.000000;,
  0.777780;0.000000;,
  0.777780;1.000000;,
  0.666670;1.000000;,
  1.741721;0.431898;,
  1.741721;0.288159;,
  -0.741721;0.288159;,
  -0.741721;0.431898;,
  0.888890;0.000000;,
  1.000000;1.000000;,
  0.888890;1.000000;,
  0.333330;0.000000;,
  0.444440;0.000000;,
  0.444440;1.000000;,
  0.333330;1.000000;,
  0.555560;0.000000;,
  0.555560;1.000000;,
  0.666670;0.000000;,
  0.666670;1.000000;,
  0.777780;0.000000;,
  0.777780;1.000000;,
  0.888890;0.000000;,
  0.888890;1.000000;,
  1.000000;0.000000;,
  0.666670;0.000000;,
  0.666670;1.000000;,
  0.777780;1.000000;,
  0.777780;0.000000;,
  0.888890;1.000000;,
  0.888890;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.111110;1.000000;,
  0.111110;0.000000;,
  0.222220;1.000000;,
  0.222220;0.000000;,
  0.333330;0.000000;,
  0.666670;1.000000;,
  0.777780;1.000000;,
  0.777780;0.000000;,
  1.741721;0.431898;,
  -0.741721;0.431898;,
  -0.741721;0.288159;,
  1.741721;0.288159;,
  0.888890;0.000000;,
  0.888890;1.000000;,
  1.000000;1.000000;,
  0.666670;0.000000;,
  0.777780;1.000000;,
  0.777780;0.000000;,
  0.888890;1.000000;,
  0.888890;0.000000;,
  1.000000;0.000000;;
 }
}