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
 37;
 14.40035;-4.53519;-4.07636;,
 0.63386;-4.17153;-3.52209;,
 0.63386;4.17151;-3.52209;,
 14.40035;4.53519;-4.07636;,
 0.00000;4.68903;0.00000;,
 0.00000;4.68903;-3.44356;,
 0.00000;0.00000;-3.44356;,
 0.00000;0.00000;0.00000;,
 0.63386;-4.17153;3.52209;,
 14.40035;-4.53519;4.07636;,
 14.40035;4.53519;4.07636;,
 0.63386;4.17151;3.52209;,
 15.05278;-5.09781;3.74378;,
 15.05278;-5.09781;-3.74378;,
 15.05278;5.09781;-3.74378;,
 15.05278;5.09781;3.74378;,
 14.40035;5.55068;-3.33059;,
 0.63386;4.79596;-3.06351;,
 0.63386;4.79596;3.06351;,
 14.40035;5.55068;3.33059;,
 14.40035;-5.55071;3.33059;,
 0.63386;-4.79596;3.06351;,
 0.63386;-4.79596;-3.06351;,
 14.40035;-5.55071;-3.33059;,
 15.05278;-5.09781;-3.74378;,
 15.05278;5.09781;-3.74378;,
 0.00000;-4.68903;-3.44356;,
 0.00000;-4.68903;3.44356;,
 0.00000;-0.00000;3.44356;,
 0.00000;4.68903;3.44356;,
 15.05278;5.09781;3.74378;,
 0.00000;4.68903;3.44356;,
 0.00000;4.68903;0.00000;,
 15.05278;-5.09781;3.74378;,
 0.00000;-4.68903;-0.00000;,
 0.00000;-4.68903;3.44356;,
 0.00000;-4.68903;-0.00000;;
 
 33;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,0,3,25;,
 4;25,3,2,5;,
 3;1,26,2;,
 3;2,6,5;,
 3;6,2,26;,
 4;26,1,0,24;,
 3;27,8,28;,
 3;28,8,29;,
 3;29,8,11;,
 4;29,11,10,15;,
 4;15,10,9,12;,
 4;12,9,8,27;,
 4;25,16,19,30;,
 4;30,19,18,31;,
 3;5,32,17;,
 3;32,31,17;,
 3;17,31,18;,
 4;5,17,16,25;,
 4;33,20,23,24;,
 4;24,23,22,26;,
 3;26,22,34;,
 3;34,22,35;,
 3;35,22,21;,
 4;35,21,20,33;,
 4;26,36,7,6;,
 4;28,29,4,7;,
 4;36,27,28,7;;
 
 MeshMaterialList {
  1;
  33;
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
   0.400000;0.400000;0.400000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  33;
  0.886072;-0.463549;-0.000000;,
  0.886064;0.463562;0.000000;,
  -0.088717;-0.325931;-0.941222;,
  -1.000000;0.000000;0.000000;,
  -0.073799;-0.436062;0.896885;,
  -0.088717;0.325934;0.941221;,
  0.131304;-0.119944;-0.984059;,
  0.131304;0.119944;-0.984059;,
  -0.090092;0.069686;-0.993492;,
  -0.067561;-0.116592;-0.990879;,
  -0.090092;-0.069686;0.993492;,
  -0.067561;0.116591;0.990879;,
  0.131304;0.119944;0.984059;,
  0.131304;-0.119944;0.984059;,
  0.171237;0.966655;0.190409;,
  0.171237;0.966655;-0.190409;,
  -0.091097;0.979131;0.181667;,
  -0.122419;0.986591;-0.107940;,
  0.171246;-0.966652;0.190415;,
  0.171246;-0.966652;-0.190415;,
  -0.122420;-0.986591;-0.107943;,
  -0.091098;-0.979130;0.181672;,
  -0.122952;0.000000;0.992413;,
  -0.166346;-0.986067;0.000000;,
  0.140617;-0.447815;-0.883000;,
  0.140617;0.447818;-0.882998;,
  -0.073800;0.436066;-0.896883;,
  -0.122952;0.000000;-0.992413;,
  0.140617;0.447818;0.882998;,
  0.140617;-0.447815;0.883000;,
  -0.166346;0.986067;0.000000;,
  -0.166346;0.986068;0.000000;,
  -0.166346;-0.986068;0.000000;;
  33;
  4;6,9,8,7;,
  4;3,3,3,3;,
  4;10,13,12,11;,
  4;0,0,1,1;,
  4;15,17,16,14;,
  4;18,21,20,19;,
  4;24,6,7,25;,
  4;25,7,8,26;,
  3;9,2,8;,
  3;8,27,26;,
  3;27,8,2;,
  4;2,9,6,24;,
  3;4,10,22;,
  3;22,10,5;,
  3;5,10,11;,
  4;5,11,12,28;,
  4;28,12,13,29;,
  4;29,13,10,4;,
  4;1,15,14,1;,
  4;28,14,16,5;,
  3;30,30,17;,
  3;30,31,17;,
  3;17,31,16;,
  4;26,17,15,25;,
  4;0,18,19,0;,
  4;24,19,20,2;,
  3;23,20,23;,
  3;23,20,32;,
  3;32,20,21;,
  4;4,21,18,29;,
  4;3,3,3,3;,
  4;3,3,3,3;,
  4;3,3,3,3;;
 }
 MeshTextureCoords {
  37;
  0.375000;1.000000;,
  0.375000;0.750000;,
  0.625000;0.750000;,
  0.625000;1.000000;,
  0.625000;0.625000;,
  0.625000;0.750000;,
  0.500000;0.750000;,
  0.500000;0.625000;,
  0.375000;0.500000;,
  0.375000;0.250000;,
  0.625000;0.250000;,
  0.625000;0.500000;,
  0.375000;0.250000;,
  0.375000;0.000000;,
  0.625000;0.000000;,
  0.625000;0.250000;,
  0.625000;1.000000;,
  0.625000;0.750000;,
  0.875000;0.750000;,
  0.875000;1.000000;,
  0.125000;1.000000;,
  0.125000;0.750000;,
  0.375000;0.750000;,
  0.375000;1.000000;,
  0.375000;1.000000;,
  0.625000;1.000000;,
  0.375000;0.750000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.875000;1.000000;,
  0.875000;0.750000;,
  0.750000;0.750000;,
  0.125000;1.000000;,
  0.250000;0.750000;,
  0.125000;0.750000;,
  0.375000;0.625000;;
 }
}
