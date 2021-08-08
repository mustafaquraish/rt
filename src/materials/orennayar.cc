#include <materials/orennayar.h>

OrenNayar::OrenNayar(float roughness, Colour col) 
  : BSDF(col) {
  // roughness = Radians(sigma);
  float sigma2 = roughness * roughness;
  A = 1.f - (sigma2 / (2.f * (sigma2 + 0.33f)));
  B = 0.45f * sigma2 / (sigma2 + 0.09f);
}


Colour OrenNayar::eval(HitRec &rec) {
  Vec3 wi = toLocalFrame(rec.wi, rec.n);
  Vec3 wo = toLocalFrame(rec.wo, rec.n);
  // std::cout << "---------------- START-----------------" << std::endl;
  // std::cout << rec.wi << "---> " <<wi << std::endl;
  // std::cout << rec.wo << "---> " <<wo << std::endl;
  // std::cout << "----------------  END -----------------" << std::endl;


  float sinThetaI = sinTheta(wi);
  float sinThetaO = sinTheta(wo);
  // Compute cosine term of Oren-Nayar model
  float maxCos = 0;
  if (sinThetaI > 1e-4 && sinThetaO > 1e-4) {
      float sinPhiI = sinPhi(wi), cosPhiI = cosPhi(wi);
      float sinPhiO = sinPhi(wo), cosPhiO = cosPhi(wo);
      float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
      maxCos = max(0, dCos);
  }

  // Compute sine and tangent terms of Oren-Nayar model
  float sinAlpha, tanBeta;
  if (absCosTheta(wi) > absCosTheta(wo)) {
      sinAlpha = sinThetaO;
      tanBeta = sinThetaI / absCosTheta(wi);
  } else {
      sinAlpha = sinThetaI;
      tanBeta = sinThetaO / absCosTheta(wo);
  }
  // printf("maxCos: %f, sinAlpha: %f, tanBeta: %f\n", maxCos , sinAlpha , tanBeta);
  return col(rec) * INV_PI * (A + B * maxCos * sinAlpha * tanBeta);
}

Colour OrenNayar::sample(HitRec &rec, RNG& rng) {
  rec.wi = rng.randomVectorCosineHemisphere(rec.n);
  float cosTheta = dot(rec.n, rec.wi);
  return (cosTheta * eval(rec)) / pdf(rec);
}

float OrenNayar::pdf(HitRec &rec) {
  return dot(rec.wi, rec.n) * INV_PI;
}
