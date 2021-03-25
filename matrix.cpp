
  class matrix{
  vector<vector<double>> create(int m , int n ){
    vector<vector<double>> mat(m, vector<double>(n,0));
    return mat;
  }

  vector<vector<int>> mult(vector<vector<double>> v1, vector<vector<double>> v2){
    if(v1[0].size() != v2.size())
      {
        printf("invalid input");
        return nullptr;
      }
      vector<vector<double>> v(v1.size(),vector<double>(v2[0].size(),0));
      for(int i = 0 ; i<v1.size(); i++){
        for(int j = 0 ; j<v2[0].size() ; j++){
          for(int k = 0 ; k<v2.size() ; k++){
            v[i][j] += v1[i][k]*v2[k][j];
          }
        }
      }
      return v;
    }
  }
