#include "materials/orennayar.h"

OrenNayar::OrenNayar(double roughness, Colour col) 
  : BSDF(col) {
  // roughness = Radians(sigma);
  double sigma2 = roughness * roughness;
  A = 1.f - (sigma2 / (2.f * (sigma2 + 0.33f)));
  B = 0.45f * sigma2 / (sigma2 + 0.09f);
}


Colour OrenNayar::eval(HitRec &rec) {
  Vec wi = toLocalFrame(rec.wi, rec.n);
  Vec wo = toLocalFrame(rec.wo, rec.n);
  // std::cout << "---------------- START-----------------" << std::endl;
  // std::cout << rec.wi << "---> " <<wi << std::endl;
  // std::cout << rec.wo << "---> " <<wo << std::endl;
  // std::cout << "----------------  END -----------------" << std::endl;


  double sinThetaI = sinTheta(wi);
  double sinThetaO = sinTheta(wo);
  // Compute cosine term of Oren-Nayar model
  double maxCos = 0;
  if (sinThetaI > 1e-4 && sinThetaO > 1e-4) {
      double sinPhiI = sinPhi(wi), cosPhiI = cosPhi(wi);
      double sinPhiO = sinPhi(wo), cosPhiO = cosPhi(wo);
      double dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
      maxCos = max(0, dCos);
  }

  // Compute sine and tangent terms of Oren-Nayar model
  double sinAlpha, tanBeta;
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
  double cosTheta = dot(rec.n, rec.wi);
  return (cosTheta * eval(rec)) / pdf(rec);
}

double OrenNayar::pdf(HitRec &rec) {
  return dot(rec.wi, rec.n) * INV_PI;
}
