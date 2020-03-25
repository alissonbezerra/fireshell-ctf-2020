package com.arconsultoria.cars.adapter

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.ImageView
import android.widget.TextView
import com.arconsultoria.cars.R
import com.arconsultoria.cars.domain.Car
import com.squareup.picasso.Picasso
import kotlinx.android.synthetic.main.listview_item.view.*

class CarAdapter(private val context: Context,
                 private val dataSource: List<Car>) : BaseAdapter() {

    private class ViewHolder {
        lateinit var name: TextView
        lateinit var year: TextView
        lateinit var image: ImageView
    }

    override fun getView(i: Int, view: View?, viewGroup: ViewGroup?): View {
        var view = view
        val viewHolder: ViewHolder

        if (view == null) {
            val inflater = LayoutInflater.from(context)
            view = inflater.inflate(R.layout.listview_item, viewGroup, false)

            viewHolder = ViewHolder()
            viewHolder.name = view!!.name
            viewHolder.year = view!!.year
            viewHolder.image = view!!.image
        } else {
            viewHolder = view.tag as ViewHolder
        }

        val car = dataSource.get(i)

        viewHolder.name.text = car.name
        viewHolder.year.text = car.year.toString()

        Picasso
            .get()
            .load(car.image)
            .into(viewHolder.image)

        view.tag = viewHolder

        return view
    }

    override fun getItem(position: Int): Any {
        return dataSource[position]
    }

    override fun getItemId(position: Int): Long {
        return position.toLong()
    }

    override fun getCount(): Int {
        return dataSource.size
    }

}

