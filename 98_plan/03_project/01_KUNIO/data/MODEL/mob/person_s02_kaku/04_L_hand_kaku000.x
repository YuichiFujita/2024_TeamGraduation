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
 57;
 7.82308;0.47875;0.92604;,
 6.99718;0.79900;1.53168;,
 8.05675;0.84397;1.49850;,
 0.60721;-0.61253;0.75342;,
 2.34217;-0.06563;0.63141;,
 2.43580;-0.70733;0.66093;,
 0.57715;0.40717;0.74670;,
 2.34181;0.58597;0.66243;,
 7.04530;-0.70766;1.55946;,
 6.99854;-0.47339;0.87423;,
 7.79633;-0.57905;0.95088;,
 2.28133;0.50188;2.50485;,
 2.28547;0.83581;1.53777;,
 0.54841;0.71902;1.55190;,
 0.48163;0.34117;2.43816;,
 2.34913;-0.30164;2.63721;,
 0.47932;-0.35588;2.55525;,
 7.01095;0.07654;2.41098;,
 7.01477;-0.35642;2.25483;,
 7.83964;-0.37856;2.33769;,
 7.92203;0.14404;2.47053;,
 6.96556;0.46978;0.87591;,
 7.70446;0.03931;0.77208;,
 6.93367;-0.00116;0.82959;,
 8.00795;0.61711;2.28471;,
 6.98504;0.51826;2.22738;,
 2.47411;-1.04171;1.53150;,
 0.62866;-1.02287;1.55388;,
 6.99718;0.79900;1.53168;,
 6.93367;-0.00116;0.82959;,
 6.99854;-0.47339;0.87423;,
 7.04530;-0.70766;1.55946;,
 7.01477;-0.35642;2.25483;,
 7.79633;-0.57905;0.95088;,
 7.04530;-0.70766;1.55946;,
 8.64253;-0.19607;1.99314;,
 8.66683;-0.02360;2.06526;,
 8.61391;-0.32861;1.07643;,
 8.54297;-0.00350;0.94902;,
 8.73595;0.34972;1.41627;,
 8.59139;0.13999;1.03911;,
 8.66683;-0.02360;2.06526;,
 8.73595;0.34972;1.41627;,
 8.05675;0.84397;1.49850;,
 7.92203;0.14404;2.47053;,
 8.71024;0.21145;1.95267;,
 8.59139;0.13999;1.03911;,
 7.82308;0.47875;0.92604;,
 8.59139;0.13999;1.03911;,
 8.59139;0.13999;1.03911;,
 8.54297;-0.00350;0.94902;,
 7.70446;0.03931;0.77208;,
 7.82308;0.47875;0.92604;,
 0.57811;0.40735;0.74730;,
 7.01095;0.07654;2.41098;,
 6.98504;0.51826;2.22738;,
 6.96556;0.46978;0.87591;;
 
 52;
 3;0,1,2;,
 3;3,4,5;,
 3;4,3,6;,
 3;6,7,4;,
 3;8,9,10;,
 3;11,12,13;,
 3;11,13,14;,
 3;15,11,14;,
 3;15,14,16;,
 3;17,18,19;,
 3;17,19,20;,
 3;21,22,23;,
 3;6,13,12;,
 3;6,12,7;,
 3;24,2,1;,
 3;25,17,20;,
 3;5,26,27;,
 3;5,27,3;,
 3;27,26,15;,
 3;27,15,16;,
 3;7,12,28;,
 3;5,4,29;,
 3;29,4,7;,
 3;26,30,31;,
 3;32,26,31;,
 3;22,33,9;,
 3;22,9,23;,
 4;19,18,34,10;,
 4;35,36,20,19;,
 4;37,35,19,33;,
 4;38,37,33,22;,
 4;38,36,35,37;,
 4;39,40,0,2;,
 4;41,42,43,44;,
 3;40,39,36;,
 4;45,39,2,24;,
 4;36,45,24,20;,
 4;46,41,44,47;,
 4;42,46,47,43;,
 4;39,45,36,48;,
 4;49,50,51,52;,
 3;50,49,36;,
 3;16,14,27;,
 4;3,27,14,6;,
 3;14,13,53;,
 4;54,15,26,32;,
 4;29,30,26,5;,
 4;54,55,11,15;,
 4;28,56,29,7;,
 4;11,55,28,12;,
 4;1,25,20,24;,
 4;1,0,22,21;;
 
 MeshMaterialList {
  8;
  52;
  0,
  2,
  2,
  2,
  0,
  2,
  2,
  2,
  2,
  0,
  0,
  0,
  2,
  2,
  0,
  0,
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
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  2,
  0,
  0,
  0,
  2,
  2,
  0,
  0,
  0,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  0,
  0;;
  Material {
   0.800000;0.674400;0.574400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.810000;0.810000;0.810000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\mob\\sukekaku002.png";
   }
  }
  Material {
   0.274480;0.409530;0.546770;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.103200;0.103200;0.103200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.571200;0.417600;0.125600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\mob\\sukekaku000.png";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  58;
  0.226935;0.679626;-0.697574;,
  0.274210;0.768267;0.578425;,
  0.137635;-0.753727;-0.642613;,
  0.151224;-0.657396;0.738216;,
  0.363949;0.922388;-0.129385;,
  -0.059978;0.019455;-0.998010;,
  -0.060356;-0.031109;-0.997692;,
  -0.047981;0.165622;0.985021;,
  -0.024227;-0.996053;0.085390;,
  -0.078844;0.989258;0.123099;,
  -0.072992;0.465189;0.882197;,
  -0.017452;-0.001356;-0.999847;,
  0.075797;0.082168;-0.993732;,
  0.237302;0.283697;0.929088;,
  0.010404;-0.005025;-0.999933;,
  0.020303;-0.240517;-0.970433;,
  0.007276;-0.644052;-0.764947;,
  0.031959;-0.748240;0.662658;,
  0.040121;-0.966139;0.254883;,
  0.037844;-0.998882;0.028315;,
  -0.014578;0.983003;-0.183011;,
  -0.045864;0.997665;-0.050616;,
  -0.039706;0.616155;0.786624;,
  -0.029714;0.835458;0.548751;,
  0.014568;0.227289;-0.973718;,
  0.003504;0.456997;-0.889461;,
  -0.001486;-0.003291;0.999994;,
  -0.031211;-0.116259;0.992728;,
  0.377449;0.598506;-0.706628;,
  0.598019;0.560480;0.572919;,
  0.272219;-0.953502;0.129351;,
  0.394949;-0.736020;0.549809;,
  0.515646;0.788980;-0.334096;,
  0.251022;0.038351;-0.967221;,
  0.496068;-0.044331;0.867151;,
  -0.997370;0.047115;-0.055066;,
  -0.050665;0.158162;0.986112;,
  -0.087827;0.942912;-0.321253;,
  -0.051504;0.959443;-0.277158;,
  -0.041514;-0.912692;-0.406533;,
  -0.069007;-0.888515;-0.453630;,
  -0.000949;-0.831706;0.555216;,
  -0.000155;-0.832288;0.554344;,
  0.102654;-0.949431;0.296719;,
  0.212300;-0.278436;-0.936698;,
  0.989616;-0.103595;-0.099644;,
  0.964552;-0.241326;-0.106775;,
  0.995081;0.049635;-0.085737;,
  0.510598;0.692229;0.510009;,
  0.935956;-0.333908;-0.111765;,
  0.921146;-0.372825;-0.111763;,
  -0.348185;-0.925323;-0.150151;,
  0.931541;-0.351353;-0.093716;,
  -0.990677;-0.019369;-0.134844;,
  -0.996999;-0.011337;-0.076586;,
  -0.995359;-0.040587;-0.087249;,
  -0.997316;-0.061695;-0.039416;,
  0.068012;-0.599088;-0.797790;;
  52;
  3;0,20,4;,
  3;6,11,15;,
  3;11,6,5;,
  3;5,24,11;,
  3;19,16,2;,
  3;23,21,9;,
  3;23,9,10;,
  3;26,36,10;,
  3;26,10,7;,
  3;27,17,3;,
  3;27,3,13;,
  3;25,12,14;,
  3;37,9,21;,
  3;37,21,38;,
  3;1,4,20;,
  3;22,27,13;,
  3;39,18,8;,
  3;39,8,40;,
  3;8,18,41;,
  3;8,41,42;,
  3;38,21,20;,
  3;15,11,14;,
  3;14,11,24;,
  3;18,16,19;,
  3;17,18,19;,
  3;12,2,16;,
  3;12,16,14;,
  4;3,17,19,43;,
  4;31,34,13,3;,
  4;30,31,3,2;,
  4;33,44,2,12;,
  4;45,46,47,47;,
  4;32,28,0,4;,
  4;48,48,4,13;,
  3;49,50,46;,
  4;29,32,4,1;,
  4;34,29,1,13;,
  4;51,51,51,51;,
  4;32,28,0,4;,
  4;50,52,46,49;,
  4;28,33,12,0;,
  3;45,49,46;,
  3;53,54,55;,
  4;56,55,54,56;,
  3;54,35,35;,
  4;27,26,18,17;,
  4;14,16,57,15;,
  4;27,22,23,26;,
  4;20,25,14,24;,
  4;23,22,20,21;,
  4;20,22,13,1;,
  4;20,0,12,25;;
 }
 MeshTextureCoords {
  57;
  0.099710;0.637760;,
  0.126380;0.677840;,
  0.124140;0.627150;,
  0.490621;15.535396;,
  1.033795;15.322724;,
  1.059461;15.587560;,
  0.486804;15.115406;,
  1.037224;15.054481;,
  0.020410;0.690390;,
  0.052590;0.685200;,
  0.033740;0.651850;,
  1.017936;15.088663;,
  1.021040;14.951226;,
  0.479550;14.986822;,
  0.456703;15.141890;,
  1.034680;15.419930;,
  0.452196;15.428823;,
  0.180930;0.680570;,
  0.201480;0.683590;,
  0.207220;0.643850;,
  0.183730;0.636380;,
  0.094400;0.681040;,
  0.070830;0.648090;,
  0.073870;0.685260;,
  0.161300;0.629850;,
  0.159440;0.679480;,
  1.069572;15.725489;,
  0.495070;15.704472;,
  2.487891;15.000236;,
  2.463767;15.329176;,
  2.481396;15.524041;,
  2.494685;15.620818;,
  2.487085;15.476007;,
  0.048570;0.647600;,
  0.236170;0.689460;,
  0.207220;0.643850;,
  0.183730;0.636380;,
  0.048570;0.647600;,
  0.070830;0.648090;,
  0.124140;0.627150;,
  0.099710;0.637760;,
  3.003286;15.350867;,
  3.026837;15.197682;,
  2.818045;14.989336;,
  2.772292;15.276504;,
  0.161300;0.629850;,
  2.980684;15.282981;,
  2.743304;15.138005;,
  0.000000;0.000000;,
  0.049860;0.318880;,
  0.035420;0.324050;,
  0.035420;0.324050;,
  0.049860;0.318880;,
  0.487104;15.115340;,
  2.488251;15.297745;,
  2.482582;15.115719;,
  2.476256;15.135535;;
 }
}