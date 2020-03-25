package com.arconsultoria.cars.activity

import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.arconsultoria.cars.domain.CommentResponse
import com.arconsultoria.cars.R
import com.arconsultoria.cars.domain.Comment
import com.arconsultoria.cars.rest.Rest
import kotlinx.android.synthetic.main.activity_comment.*
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory

class CommentActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_comment)

        btn_send.setOnClickListener {
            send_comment()
        }
    }

    fun send_comment() {
        val retrofit = Retrofit.Builder()
            .baseUrl(resources.getString(R.string.url_api))
            .addConverterFactory(GsonConverterFactory.create())
            .build()

        val service = retrofit.create(Rest::class.java)
        val comment = Comment(edt_name.text.toString(), edt_message.text.toString())
        val call = service.postComment(comment)
        call.enqueue(object : Callback<CommentResponse> {
            override fun onFailure(call: Call<CommentResponse>, t: Throwable) {
                Toast.makeText(baseContext, "Error posting comment!", Toast.LENGTH_LONG).show()
            }

            override fun onResponse(
                call: Call<CommentResponse>,
                response: Response<CommentResponse>
            ) {
                if (response.code() != 200) {
                    Toast.makeText(baseContext, "Error posting comment!", Toast.LENGTH_LONG).show()
                    return
                }

                Toast.makeText(baseContext, response.body()?.message, Toast.LENGTH_LONG).show()
                finish()
            }
        })
    }

}