FROM	alpine:latest

ENV		TIMEZONE Europe/Amsterdam

RUN		apk update && \
		apk upgrade && \
		apk add --update openssl nginx && \
		apk add --update tzdata && \
		cp /usr/share/zoneinfo/${TIMEZONE} /etc/localtime && \
		echo "${TIMEZONE}" > /etc/timezone && \
		mkdir /etc/nginx/certificates /etc/nginx/locs /etc/nginx/configs && \
		openssl req \
			-x509 \
			-newkey rsa:2048 \
			-keyout /etc/nginx/certificates/key.pem \
			-out /etc/nginx/certificates/cert.pem \
			-days 365 \
			-nodes \
			-subj /CN=localhost && \
		mkdir /www && \
		apk del tzdata && \
		rm -rf /var/cache/apk/*

ADD	/loc /etc/nginx/locs/
# ADD	/configs/ /etc/nginx//configs/

# VOLUME ["/var/log/nginx", "/www"]

# ENTRYPOINT ["/usr/sbin/nginx", "-g", "daemon off;"]