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
 32;
 2.55761;0.00000;-2.55761;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;-3.61701;,
 0.00000;0.00000;-3.61701;,
 -0.00002;102.42288;-3.61701;,
 2.55760;102.42288;-2.55761;,
 -0.00002;102.42288;-3.61701;,
 -0.00002;102.42288;0.00000;,
 3.61701;0.00000;0.00000;,
 3.61699;102.42288;0.00000;,
 2.55761;0.00000;2.55761;,
 2.55761;0.00000;2.55761;,
 2.55760;102.42288;2.55761;,
 2.55760;102.42288;2.55761;,
 0.00000;0.00000;3.61701;,
 0.00000;0.00000;3.61701;,
 -0.00002;102.42288;3.61701;,
 -0.00002;102.42288;3.61701;,
 -2.55761;0.00000;2.55761;,
 -2.55761;0.00000;2.55761;,
 -2.55763;102.42288;2.55761;,
 -2.55763;102.42288;2.55761;,
 -3.61701;0.00000;0.00000;,
 -3.61701;0.00000;0.00000;,
 -3.61703;102.42288;0.00000;,
 -3.61703;102.42288;0.00000;,
 -2.55761;0.00000;-2.55761;,
 -2.55761;0.00000;-2.55761;,
 -2.55763;102.42288;-2.55761;,
 -2.55763;102.42288;-2.55761;,
 -2.55761;0.00000;-2.55761;,
 -2.55763;102.42288;-2.55761;;
 
 24;
 3;0,1,2;,
 4;0,3,4,5;,
 3;5,6,7;,
 3;8,1,0;,
 4;8,0,5,9;,
 3;9,5,7;,
 3;10,1,8;,
 4;11,8,9,12;,
 3;13,9,7;,
 3;14,1,10;,
 4;15,11,12,16;,
 3;17,13,7;,
 3;18,1,14;,
 4;19,15,16,20;,
 3;21,17,7;,
 3;22,1,18;,
 4;23,19,20,24;,
 3;25,21,7;,
 3;26,1,22;,
 4;27,23,24,28;,
 3;29,25,7;,
 3;2,1,26;,
 4;3,30,31,4;,
 3;6,29,7;;
 
 MeshMaterialList {
  1;
  24;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\èAêEçÏïi\\2024_01Action\\202401_Action\\data\\TEXTURE\\enemy\\practice\\practiceUV.png";
   }
  }
 }
 MeshNormals {
  10;
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  -0.000000;0.000000;-1.000000;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707107;,
  -0.000000;0.000000;1.000000;,
  -0.707107;-0.000000;0.707107;,
  -1.000000;-0.000000;0.000000;,
  -0.707107;-0.000000;-0.707107;;
  24;
  3;0,0,0;,
  4;3,2,2,3;,
  3;1,1,1;,
  3;0,0,0;,
  4;4,3,3,4;,
  3;1,1,1;,
  3;0,0,0;,
  4;5,4,4,5;,
  3;1,1,1;,
  3;0,0,0;,
  4;6,5,5,6;,
  3;1,1,1;,
  3;0,0,0;,
  4;7,6,6,7;,
  3;1,1,1;,
  3;0,0,0;,
  4;8,7,7,8;,
  3;1,1,1;,
  3;0,0,0;,
  4;9,8,8,9;,
  3;1,1,1;,
  3;0,0,0;,
  4;2,9,9,2;,
  3;1,1,1;;
 }
 MeshTextureCoords {
  32;
  0.274850;0.768240;,
  0.263260;0.773260;,
  0.268090;0.761070;,
  0.274880;0.758140;,
  0.629510;0.759270;,
  0.629480;0.769380;,
  0.636280;0.762250;,
  0.641030;0.774470;,
  0.274810;0.778340;,
  0.629450;0.779480;,
  0.268010;0.785470;,
  0.274780;0.788450;,
  0.629410;0.789590;,
  0.636200;0.786650;,
  0.258430;0.785440;,
  0.274750;0.798550;,
  0.629380;0.799690;,
  0.645780;0.786680;,
  0.251680;0.778270;,
  0.274720;0.808660;,
  0.629350;0.809800;,
  0.652590;0.779560;,
  0.251710;0.768170;,
  0.274690;0.818760;,
  0.629320;0.819900;,
  0.652620;0.769450;,
  0.258510;0.761040;,
  0.274650;0.828870;,
  0.629280;0.830000;,
  0.645860;0.762280;,
  0.274910;0.748030;,
  0.629540;0.749170;;
 }
}
