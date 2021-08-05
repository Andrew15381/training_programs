package ga.hopelessness.test.server;

import android.content.*;
import android.graphics.*;
import android.graphics.drawable.*;
import android.os.*;
import android.widget.*;

/*Загрузчик картинок*/
public final class LoadImageAsyncTask extends AsyncTask<String, Void, Drawable>
{
	/*Контекст приложения*/
	private Context context;
	/*Исключение*/
	private Exception exception = null;

	/*Создает новый загрузчик.
	 context - контекст приложения*/
	public LoadImageAsyncTask(Context context)
	{
		this.context = context;
	}

	/*Загружает картинки.
	 sources - адреса картинок*/
	@Override
	protected Drawable doInBackground(String ...sources)
	{
		Drawable image=new ColorDrawable(Color.TRANSPARENT);
		try
		{
			image = Server.loadImage(sources[0]);
		}
		catch (Exception e)
		{
			exception = e;
		}
		return image;
	}

	/*Выводит сообщение об ошибке.
	  image - загруженная картинка*/
	@Override
	protected void onPostExecute(Drawable image)
	{
		if (exception != null)
		{
			Toast.makeText(context, "Проблема с интернетом", Toast.LENGTH_LONG).show();
		}
	}
}
