

// a single axis Kalman filter class for filtering a single axis or rotational data i.e filter gyro data for yaw and pitch
// create an instance of the Kalman filter class like this KalmanFilter my_filter = KalmanFilter(your_Q, your_R);
// inside your update loop do float filtered_gyro = my_filter.update(your_raw_gyro_data_here);
// if you know the feedback Matrix B of your rocket you can do: float filtered_gyro = my_filter.update(your_last_servo_value,your_raw_gyro_data_here);
class KalmanFilter{

  public:
    //Q is basically how much we trust the dynamics of the system... a lower Q means more aggressive filtering
    //R you can do the covariance of your sensor noise or the co variance of the noise induced by your motor firing
    KalmanFilter(float Q,float R);//class constructor

    float update(float U = 0.0, float Y); // this is where the magic happens! 

    // this function allows you to easily play with different Q values to tweak the sensitivity of your system
    // feeding in my_filter.changeQR(1) will cause the system to become 10% more dependent on the dynamics model 
    // changeQR(5) is a 50% increase and so on
    void changeQR(int direction);
  private:
    float A = 1.0f;
    float B = 1.0; // You will need to change this value to your specific B value if you want feedback in the filter
    //if you dont know what that means just ignore it the filter will automatically set ignore this value for you
    float C = 1.0f;
    float D = 0.0f;
    float x_hat = 0.0f;
    float P = 0.0f;
    float Q;
    float R;
};

/*Waiver of Liability

This agreement releases Jmack/Orion aerospace from all liability relating to injuries that may occur using this code.
 By downloading, I agree to hold Jmack/Orion Aerospace entirely free from any liability,
  including financial responsibility for injuries incurred, regardless of whether injuries are a result of negligence or otherwise.

By downloading or using this code, you forfeit all right to bring a suit against Jmack/Orion aerospace for any reason.
You must also obey all necessary safety precautions associated with this activity.
*/ 
