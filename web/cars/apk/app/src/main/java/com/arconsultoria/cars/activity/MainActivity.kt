package com.arconsultoria.cars.activity

import android.content.Intent
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.arconsultoria.cars.R
import com.arconsultoria.cars.rest.Rest
import com.arconsultoria.cars.adapter.CarAdapter
import com.arconsultoria.cars.domain.Car
import kotlinx.android.synthetic.main.activity_main.*
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        listView.setOnItemClickListener { _, _, position, _ ->
            val car = listView.adapter.getItem(position) as Car
            val intent = Intent(baseContext, DetailsActivity::class.java)
            intent.putExtra("id", car.id)
            startActivity(intent)
        }

        val retrofit = Retrofit.Builder()
                .baseUrl(resources.getString(R.string.url_api))
                .addConverterFactory(GsonConverterFactory.create())
                .build()

        val service = retrofit.create(Rest::class.java)
        val call = service.getCars()
        call.enqueue(object : Callback<List<Car>> {
            override fun onFailure(call: Call<List<Car>>, t: Throwable) {
                Toast.makeText(baseContext, "Error retrieving cars!", Toast.LENGTH_LONG).show()
            }

            override fun onResponse(call: Call<List<Car>>, response: Response<List<Car>>) {
                if (response.code() != 200) {
                    Toast.makeText(baseContext, "Error retrieving cars!", Toast.LENGTH_LONG).show()
                    return
                }

                val adapter = CarAdapter(
                    baseContext,
                    response.body().orEmpty()
                )
                listView.adapter = adapter
            }
        })
    }
}