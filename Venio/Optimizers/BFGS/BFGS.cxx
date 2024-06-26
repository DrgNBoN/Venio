#include "BFGS.hxx"

void BFGS::updateWeights(double learning_speed, double epoch)
{
    for(int i = 0;i < _network.getLayersSize()-1;i++){

        _layer_weights_gradient = _network.getLayerWeightsGradient(i);
        _layer_weights = _network.getLayerWeights(i);

        _old_gradient[i] = Eigen::Map<Vector>(_layer_weights_gradient.data(),
                                              _layer_weights_gradient.size());

        _old_weights[i] = Eigen::Map<Vector>(_layer_weights.data(),
                                             _layer_weights.size());

        _new_weights[i] = K::sub(_old_weights[i], K::scalarMultiply(learning_speed,
                                                  K::multiply(_inversed_hessian[i], _old_gradient[i])));
                                               


        _network.setLayerWeights(i,Eigen::Map<Matrixd>(_new_weights[i].data(),
                                                       _layer_weights.rows(),
                                                       _layer_weights.cols()));
    }
    _network.forwardPropogation();

    //WARNING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    _network.backPropogation();

    for(int i = 0;i < _network.getLayersSize()-1;i++){

        _new_gradient[i] = Eigen::Map<Vector>(_network.getLayerWeightsGradient(i).data(),
                                              _network.getLayerWeightsGradient(i).size());



        s = K::sub(_new_weights[i],_old_weights[i]);

        y = K::sub(_new_gradient[i],_old_gradient[i]);

        ro = 1 / K::dot(y,s);


        Matrixd a = K::sub(I[i] , K::scalarMultiply(ro , K::multiply(s , K::transpose(y) ) ) );
        Matrixd b = K::sub(I[i] , K::scalarMultiply(ro , K::multiply(y , K::transpose(s) ) ) );


        _inversed_hessian[i] = K::multiply(K::multiply(a,_inversed_hessian[i]),b) + K::scalarMultiply(ro, K::multiply(s, K::transpose(s) ) ) ;

    }
}

BFGS::~BFGS() {
    delete[] _inversed_hessian;
    delete[] I;
    delete[] _old_gradient;
    delete[] _new_gradient;
    delete[] _old_weights;
    delete[] _new_weights;

}
