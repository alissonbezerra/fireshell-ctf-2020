package com.arconsultoria.cars.activity

import android.content.Intent
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.arconsultoria.cars.R
import com.arconsultoria.cars.rest.Rest
import com.arconsultoria.cars.domain.Car
import com.squareup.picasso.Picasso
import kotlinx.android.synthetic.main.activity_details.*
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory

class DetailsActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_details)

        val actionBar = supportActionBar
        actionBar!!.title = "Details"
        actionBar.setDisplayHomeAsUpEnabled(true)

        val id = intent.getIntExtra("id", 0)
        getCar(id)

        btn_comment.setOnClickListener {
            val intent = Intent(baseContext, CommentActivity::class.java)
            startActivity(intent)
        }
    }

    override fun onSupportNavigateUp(): Boolean {
        onBackPressed()
        return true
    }

    fun getCar(id: Int) {
        val retrofit = Retrofit.Builder()
            .baseUrl(resources.getString(R.string.url_api))
            .addConverterFactory(GsonConverterFactory.create())
            .build()

        val service = retrofit.create(Rest::class.java)
        val call = service.getCar(id)

        call.enqueue(object : Callback<Car> {
            override fun onFailure(call: Call<Car>, t: Throwable) {
                Toast.makeText(baseContext, "Error retrieving car!", Toast.LENGTH_LONG).show()
            }

            override fun onResponse(call: Call<Car>, response: Response<Car>) {
                if (response.code() != 200) {
                    Toast.makeText(baseContext, "Error retrieving car!", Toast.LENGTH_LONG).show()
                    return
                }

                details_name.text = response.body()?.name
                details_year.text = response.body()?.year.toString()

                Picasso
                    .get()
                    .load(response.body()?.image)
                    .into(details_image)
            }
        })
    }
}