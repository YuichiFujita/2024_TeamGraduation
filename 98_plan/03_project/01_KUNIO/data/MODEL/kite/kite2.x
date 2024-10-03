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
 10;
 0.00000;41.07547;-21.10655;,
 -194.15056;41.07547;-6.02017;,
 -194.15056;196.40321;24.03637;,
 0.00000;196.40321;-6.02017;,
 194.15056;196.40321;24.03637;,
 194.15056;41.07547;-6.02017;,
 194.15056;-114.25235;13.37024;,
 0.00000;-198.49916;-6.02017;,
 -194.15056;-114.25235;13.37024;,
 0.00000;41.07547;-6.02017;;
 
 8;
 4;0,1,2,3;,
 4;0,3,4,5;,
 4;0,5,6,7;,
 4;0,7,8,1;,
 4;3,2,1,9;,
 4;3,9,5,4;,
 4;7,6,5,9;,
 4;7,9,1,8;;
 
 MeshMaterialList {
  2;
  8;
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\kite02.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\kite00.jpg";
   }
  }
 }
 MeshNormals {
  18;
  -0.113890;0.142356;-0.983242;,
  0.113890;0.142356;-0.983242;,
  -0.101485;-0.092998;-0.990481;,
  0.101485;-0.092998;-0.990481;,
  -0.108444;0.024852;-0.993792;,
  0.108444;0.024852;-0.993792;,
  0.000000;0.024999;-0.999687;,
  0.000000;0.143288;-0.989681;,
  0.000000;-0.093481;-0.995621;,
  0.000000;-0.016321;0.999867;,
  0.000000;-0.094881;0.995489;,
  0.075690;-0.094608;0.992633;,
  0.062968;-0.016289;0.997883;,
  -0.062968;-0.016289;0.997883;,
  -0.075690;-0.094608;0.992633;,
  0.000000;0.062211;0.998063;,
  -0.049847;0.062133;0.996822;,
  0.049847;0.062133;0.996822;;
  8;
  4;6,4,0,7;,
  4;6,7,1,5;,
  4;6,5,3,8;,
  4;6,8,2,4;,
  4;10,11,12,9;,
  4;10,9,13,14;,
  4;15,16,13,9;,
  4;15,9,12,17;;
 }
 MeshTextureCoords {
  10;
  0.500000;0.500000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.500000;0.500000;;
 }
}
